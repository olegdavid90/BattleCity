#include "LevelMap.hpp"
#include "system/log.hpp"
#include "game/states/Level.hpp"
#include "game/objects/Ice.hpp"
#include "game/objects/Tree.hpp"
#include "game/objects/Eagle.hpp"
#include "game/objects/Water.hpp"
#include "game/objects/Border.hpp"
#include "game/objects/BrickWall.hpp"
#include "game/objects/BigExplosion.hpp"
#include "game/objects/ConcreteWall.hpp"
#include "game/ResourcesManager.hpp"


//----------------------------------------------------------------------------//
game::LevelMap::LevelMap(const Level* parent_level)
{
	this->initLevelMap(parent_level->getIndex());
	this->initBorders();
	this->m_swapEagleWallCount = 0;
	this->m_isBreakEagleWall   = true;
	this->m_EagleWallTimer.setCallback(
		[this]()
		{
			if (this->m_isBreakEagleWall == false)
			{
				const tps::Vec2i wallTilePosition{
					this->m_pEagleWall[this->m_swapEagleWallCount]->getPosition()
				};
				this->m_pEagleWall[this->m_swapEagleWallCount].reset();
				this->m_pEagleWall[this->m_swapEagleWallCount]
					= std::make_unique<BrickWall>(wallTilePosition);
				++this->m_swapEagleWallCount;

			}
			this->swapEagleWall();
			if (this->m_swapEagleWallCount < this->m_pEagleWall.size())
			{
				this->m_EagleWallTimer.start(FPS / 4);
			}
		}
	);
}

//----------------------------------------------------------------------------//
void game::LevelMap::render() const
{
	for (const auto& currentBorder : this->m_pBorders)
	{
		currentBorder->render();
	}

	for (auto& currentStaticObject : this->m_pStaticObjects)
	{
		if (currentStaticObject != nullptr)
		{
			currentStaticObject->render();
		}
	}
}

//----------------------------------------------------------------------------//
void game::LevelMap::renderTrees() const
{
	for (const auto& currentTree : this->m_pTrees)
	{
		currentTree->render();
	}
}

//----------------------------------------------------------------------------//
void game::LevelMap::update()
{
	for (auto currentWater : this->m_pWaters)
	{
		currentWater->update();
	}
	this->m_EagleWallTimer.update();
}

//----------------------------------------------------------------------------//
void game::LevelMap::updateCollisionObjects(
	const tps::IntRect& global_bounds,
	std::vector<class StaticObject*>& objects
) const
{
	tps::IntRect convertedBounds{ global_bounds };
	convertedBounds.left -=
		this->m_pBorders[static_cast<unsigned>(EBorderType::Left)]->getSize().x;
	convertedBounds.top -= 
		this->m_pBorders[static_cast<unsigned>(EBorderType::Top)]->getSize().y;

	int startX{ std::max(0, convertedBounds.left / BLOCK_SIZE) };
	int startY{ std::max(0, convertedBounds.top / BLOCK_SIZE) };

	int endX{ (convertedBounds.left + convertedBounds.width + BLOCK_SIZE - 1) / BLOCK_SIZE };
	int endY{ (convertedBounds.top + convertedBounds.height + BLOCK_SIZE - 1) / BLOCK_SIZE };

	if (convertedBounds.left < 0)
	{
		objects.push_back(this->m_pBorders[static_cast<unsigned>(EBorderType::Left)].get());
	}
	else if (endX > BLOCKS_PER_GAME_WIDTH)
	{
		objects.push_back(this->m_pBorders[static_cast<unsigned>(EBorderType::Right)].get());
		endX = BLOCKS_PER_GAME_WIDTH;
	}

	if (convertedBounds.top < 0)
	{
		objects.push_back(this->m_pBorders[static_cast<unsigned>(EBorderType::Top)].get());
	}
	else if (endY > BLOCKS_PER_GAME_HEIGHT)
	{
		objects.push_back(this->m_pBorders[static_cast<unsigned>(EBorderType::Bottom)].get());
		endY = BLOCKS_PER_GAME_HEIGHT;
	}

	for (int noColumn{ startX }; noColumn < endX; ++noColumn)
	{
		for (int noRow{ startY }; noRow < endY; ++noRow)
		{
			const int noObject{ noRow * BLOCKS_PER_GAME_WIDTH + noColumn };
			auto currentObject{ this->m_pStaticObjects[noObject].get() };

			if (currentObject != nullptr)
			{
				objects.push_back(currentObject);
			}
		}
	}
}

//----------------------------------------------------------------------------//
void game::LevelMap::concreteWallsEagle()
{
	if (this->m_isBreakEagleWall)
	{
		this->swapEagleWall();
	}
	this->m_swapEagleWallCount = 0;
	this->m_EagleWallTimer.start(16 * FPS);
}

//----------------------------------------------------------------------------//
void game::LevelMap::correctLevelMap(std::vector<std::string>& level_desctiption)
{
	this->m_pEagleWall.reserve(std::size(EMPTY_TILE));

	if (level_desctiption.size() > BLOCKS_PER_GAME_HEIGHT)
	{
		level_desctiption.resize(BLOCKS_PER_GAME_HEIGHT);
	}
	for (auto& currentRow : level_desctiption)
	{
		if (currentRow.length() > BLOCKS_PER_GAME_WIDTH)
		{
			currentRow.resize(BLOCKS_PER_GAME_WIDTH);
		}
		while (currentRow.length() < BLOCKS_PER_GAME_WIDTH)
		{
			currentRow += '.';
		}
	}
	for (unsigned index{}; index < std::size(EMPTY_TILE); ++index)
	{
		const unsigned noRow   { EMPTY_TILE[index] / BLOCKS_PER_GAME_WIDTH };
		const unsigned noColumn{ EMPTY_TILE[index] % BLOCKS_PER_GAME_WIDTH };
		level_desctiption[noRow][noColumn] = '.';
	}
	for (unsigned index{}; index < std::size(this->EAGLE_WALL_TILES); ++index)
	{
		const unsigned noRow   { EAGLE_WALL_TILES[index] / BLOCKS_PER_GAME_WIDTH };
		const unsigned noColumn{ EAGLE_WALL_TILES[index] % BLOCKS_PER_GAME_WIDTH };
		level_desctiption[noRow][noColumn] = 'B';

		const tps::Vec2i eagleWallTilePosition{
			static_cast<int>(noColumn + 2) * BLOCK_SIZE,
			static_cast<int>(noRow    + 1) * BLOCK_SIZE,
		};
		this->m_pEagleWall.emplace_back(
			std::make_unique<ConcreteWall>(eagleWallTilePosition)
		);
	}
}

//----------------------------------------------------------------------------//
void game::LevelMap::initLevelMap(unsigned level_index)
{
	auto levelDescription{ ResourcesManager::getLevelDescription(level_index) };

	this->correctLevelMap(levelDescription);
	this->m_pStaticObjects.reserve(
		static_cast<std::size_t>(BLOCKS_PER_GAME_WIDTH) * BLOCKS_PER_GAME_HEIGHT
	);

	int topOffset{ BLOCK_SIZE };
	for (const auto& currentRow : levelDescription)
	{
		int leftOffset{ BLOCK_SIZE * 2 };
		for (char currentElement : currentRow)
		{
			switch (currentElement)
			{
			case '.':
				this->m_pStaticObjects.push_back(nullptr);
				break;
			case 'B':
				this->m_pStaticObjects.emplace_back(
					std::make_unique<BrickWall>(tps::Vec2i{ leftOffset, topOffset })
				);
				break;
			case 'C':
				this->m_pStaticObjects.emplace_back(
					std::make_unique<ConcreteWall>(tps::Vec2i{ leftOffset, topOffset })
				);
				break;
			case 'I':
				this->m_pStaticObjects.emplace_back(
					std::make_unique<Ice>(tps::Vec2i{ leftOffset, topOffset })
				);
				break;
			case 'T':
				this->m_pStaticObjects.push_back(nullptr);
				this->m_pTrees.emplace_back(
					std::make_unique<Tree>(tps::Vec2i{ leftOffset, topOffset })
				);
				break;
			case 'W':
				this->m_pStaticObjects.emplace_back(
					std::make_unique<Water>(tps::Vec2i{ leftOffset, topOffset })
				);
				this->m_pWaters.push_back(
					static_cast<Water*>(this->m_pStaticObjects.back().get())
				);
				break;
			default:
				LOG_WARN("MAP::Unknown game object's description: '{}'", currentElement);
				this->m_pStaticObjects.push_back(nullptr);
			}

			leftOffset += BLOCK_SIZE;
		}

		topOffset += BLOCK_SIZE;
	}
}

//----------------------------------------------------------------------------//
void game::LevelMap::swapEagleWall()
{
	for (unsigned index{}; index < std::size(EAGLE_WALL_TILES); ++index)
	{
		std::swap(
			this->m_pStaticObjects[EAGLE_WALL_TILES[index]],
			this->m_pEagleWall[index]
		);
	}
	this->m_isBreakEagleWall = !this->m_isBreakEagleWall;

}

//----------------------------------------------------------------------------//
void game::LevelMap::initBorders()
{
	this->m_pBorders[static_cast<unsigned>(EBorderType::Top)] =
		std::make_unique<Border>(
			tps::Vec2i{},
			tps::Vec2i{ WINDOW_WIDTH, BLOCK_SIZE }
	);

	this->m_pBorders[static_cast<unsigned>(EBorderType::Bottom)] =
		std::make_unique<Border>(
			tps::Vec2i{ 0,            WINDOW_HEIGHT - BLOCK_SIZE },
			tps::Vec2i{ WINDOW_WIDTH, BLOCK_SIZE }
		);

	this->m_pBorders[static_cast<unsigned>(EBorderType::Left)] =
		std::make_unique<Border>(
			tps::Vec2i{ 0,              BLOCK_SIZE },
			tps::Vec2i{ BLOCK_SIZE * 2, GAME_HEIGHT }
		);

	this->m_pBorders[static_cast<unsigned>(EBorderType::Right)] =
		std::make_unique<Border>(
			tps::Vec2i{ GAME_WIDTH + BLOCK_SIZE * 2, BLOCK_SIZE },
			tps::Vec2i{ BLOCK_SIZE * 4,              GAME_HEIGHT }
	);
}
