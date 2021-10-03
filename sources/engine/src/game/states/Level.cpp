#include "Level.hpp"
#include "system/log.hpp"
#include "system/utils.hpp"
#include "system/FramesTimer.hpp"
#include "renderer/Sprite2D.hpp"
#include "game/Core.hpp"
#include "game/states/LevelStart.hpp"
#include "game/states/LevelFinish.hpp"
#include "game/InputHandler.hpp"
#include "game/ResourcesManager.hpp"
#include "game/objects/Bonus.hpp"
#include "game/objects/Eagle.hpp"
#include "game/objects/Enemies.hpp"
#include "game/objects/BigExplosion.hpp"
#include "game/objects/tanks/EnemyTank.hpp"
#include "game/objects/tanks/PlayerTank.hpp"

#include <typeinfo>
#include <algorithm>

namespace game {

//----------------------------------------------------------------------------//
Level::Level(Core* pCore)
	: GameState(pCore)
	, m_PhysicsEngine{ this }
	, m_pLevelMap    { std::make_unique<LevelMap>(this) }
	, m_pEnemies     { std::make_unique<Enemies>(this) }
	, m_pLevelGUI    { std::make_unique<LevelGUI>(this) }
	, m_pBonus       { nullptr }
	, m_pEagle       { std::make_unique<Eagle>(this) }
	, m_pLevelStart  { std::make_unique<LevelStart>(this) }
	, m_pLevelFinish { nullptr }
	, m_pGameOverText{ std::make_unique<renderer::Text2D>("pixel_font")}
	, m_pPlayer      { this->m_pCore->getPlayer() }
	, m_eLevelState  { ELevelState::Start }
	, m_FramesCount  { 0 }
	, m_isGameOver   { false }
{
	this->initText();
	this->m_DestroyedEnemiesCounts.fill(0);
	this->m_pPlayer->setLevel(this);
	this->m_PhysicsEngine.addDynamicObject(this->m_pPlayer);
	this->m_FinishTimer.setCallback(
		[this] { this->m_eLevelState = ELevelState::Finish; }
	);
}

//----------------------------------------------------------------------------//
void Level::render() const
{
	this->m_pLevelMap->render();
	this->m_pEagle->render();
	this->m_pLevelGUI->render(this->m_pEnemies->getPassiveTanksCount());

	for (const auto& currentDynamicObject : this->m_PhysicsEngine.getDynamicObjects())
	{
		currentDynamicObject->render();
	}

	this->m_pLevelMap->renderTrees();
	if (this->m_pBonus != nullptr)
	{
		this->m_pBonus->render();
	}

	for (const auto& currentExplosion : this->m_BigExplosions)
	{
		currentExplosion->render();
	}
	if (this->isGameOver())
	{
		this->m_pGameOverText->render();
	}

	switch (this->m_eLevelState)
	{
	case ELevelState::Start:
		this->m_pLevelStart->render();
		break;
	case ELevelState::Finish:
		this->m_pLevelFinish->render();
		break;
	default: break;
	}
}

//----------------------------------------------------------------------------//
void Level::updateInput()
{
	if (this->isGameOver() == false)
	{
		this->m_pPlayer->updateInput();
	}
}

//----------------------------------------------------------------------------//
void Level::update()
{
	if (this->m_eLevelState == ELevelState::Start)
	{
		this->m_pLevelStart->update();
		return;
	}

	for (const auto& currentExplosion : this->m_BigExplosions)
	{
		currentExplosion->update();
	}
	if (this->isGameOver())
	{
		if (this->m_pGameOverText->getPosition().y
			> (WINDOW_HEIGHT - this->m_pGameOverText->getSize().y) / 2)
		{
			this->m_pGameOverText->setPosition(
				this->m_pGameOverText->getPosition().x,
				this->m_pGameOverText->getPosition().y - 1
			);
		}
	}
 
	this->m_FinishTimer.update();
	if (this->m_pBonus != nullptr)
	{
		this->m_pBonus->update();
	}

	if (this->m_eLevelState == ELevelState::Finish)
	{
		this->m_pLevelFinish->update();
	}
	else
	{
        this->m_pLevelGUI->update();
		this->m_pEnemies->update();
		this->m_pLevelMap->update();
		this->m_PhysicsEngine.update();
	}
}

//----------------------------------------------------------------------------//
const tps::Vec2i& Level::getPlayerPosition() const
{
	return this->m_pPlayer->getPosition();
}

//----------------------------------------------------------------------------//
const std::array<unsigned, 5>& Level::getDestroyedEnemiesCounts() const
{
	return this->m_DestroyedEnemiesCounts;
}

//----------------------------------------------------------------------------//
bool Level::isGameOver() const
{
	return this->m_isGameOver;
}

//----------------------------------------------------------------------------//
void Level::run()
{
	this->m_eLevelState = ELevelState::Active;
}

//----------------------------------------------------------------------------//
void Level::finish()
{
	if (this->m_eLevelState != ELevelState::Finish)
	{
		this->saveConfig();
		this->m_pLevelFinish = std::make_unique<LevelFinish>(this);
		this->m_FinishTimer.start(3);
	}
}

//----------------------------------------------------------------------------//
void Level::startGameOver()
{
	this->m_pPlayer->stop();
	this->m_isGameOver = true;
	this->m_pLevelFinish = std::make_unique<LevelFinish>(this);
	this->m_FinishTimer.start(5);
}

//----------------------------------------------------------------------------//
void Level::finishGameOver()
{
	this->m_pCore->gameOver();
}

//----------------------------------------------------------------------------//
void Level::setState(ELevelState state)
{
	this->m_eLevelState = state;
}

//----------------------------------------------------------------------------//
void Level::createBonus()
{
	this->m_pBonus = std::make_unique<Bonus>(this);

	const tps::IntRect playerBounds{ this->m_pPlayer->getGlobalBounds() };
	const tps::IntRect eagleBounds { this->m_pEagle->getGlobalBounds() };
	tps::IntRect       bonusBounds { this->m_pBonus->getGlobalBounds() };

	while (bonusBounds.intersects(playerBounds) || bonusBounds.intersects(eagleBounds))
	{
		this->m_pBonus->setPosition(Bonus::getRandomBonusPosition());
		bonusBounds = this->m_pBonus->getGlobalBounds();
	}
}

//----------------------------------------------------------------------------//
void Level::concreteWallsEagle()
{
	this->m_pLevelMap->concreteWallsEagle();
}

//----------------------------------------------------------------------------//
void Level::freezeEnemies()
{
	this->m_pEnemies->freezeEnemies();
}

//----------------------------------------------------------------------------//
void Level::removeActiveEnemies()
{
	this->m_pEnemies->removeActiveEnemies();
}

//----------------------------------------------------------------------------//
void Level::resetBonus()
{
	this->m_pBonus.reset();
}

//----------------------------------------------------------------------------//
void Level::addActiveDynamicObject(DynamicObject* pGame_object)
{
	this->m_PhysicsEngine.addDynamicObject(pGame_object);
}

//----------------------------------------------------------------------------//
void Level::setExplosion(const tps::Vec2i& position, unsigned score)
{
	this->m_BigExplosions.emplace_back(
		std::make_unique<BigExplosion>(position, score)
	);

	if (score != 0)
	{
		this->m_pPlayer->setScore(this->m_pPlayer->getScore() + score);
		++this->m_DestroyedEnemiesCounts[score/100-1];
		++this->m_DestroyedEnemiesCounts.back();
	}
}

//----------------------------------------------------------------------------//
void Level::nextLevel()
{
	this->m_pCore->startNextLevel();
}

//----------------------------------------------------------------------------//
unsigned Level::getIndex() const
{
	return this->m_pCore->getLevelIndex();
}

//----------------------------------------------------------------------------//
unsigned Level::getPlayerScore() const
{
	return this->m_pPlayer->getScore();
}

//----------------------------------------------------------------------------//
int Level::getPlayerHealth() const
{
	return this->m_pPlayer->getHealth();
}

//----------------------------------------------------------------------------//
unsigned Level::getPlayerSubType() const
{
	return this->m_pPlayer->getSubType();
}

//----------------------------------------------------------------------------//
std::vector<StaticObject*>
Level::getCollisionObjects(const tps::IntRect& global_bounds) const
{
	std::vector<StaticObject*> objects{};
	this->m_pLevelMap->updateCollisionObjects(global_bounds, objects);

	objects.push_back(this->m_pEagle.get());
	if (this->m_pBonus != nullptr)
	{
		objects.push_back(this->m_pBonus.get());
	}

	return objects;
}

//----------------------------------------------------------------------------//
void Level::saveConfig()
{
	std::ostringstream osstrm{};
	osstrm << "\nhigh_score      " << ResourcesManager::getSavedValue("high_score")
		   << "\nlast_level      " << this->getIndex()
		   << "\nplayer_subtype  " << this->getPlayerSubType()
		   << "\nplayer_health   " << this->getPlayerHealth()
		   << "\nplayer_score    " << this->getPlayerScore()
		   << '\n';

	ResourcesManager::saveConfig(osstrm.str());
}

//----------------------------------------------------------------------------//
void Level::initText()
{
	this->m_pGameOverText->setString("GAME\nOVER");
	this->m_pGameOverText->setFillColor(216, 40, 0);
	this->m_pGameOverText->setCharacterSize(static_cast<unsigned>(BLOCK_SIZE * 2));
	this->m_pGameOverText->setPosition(
		(WINDOW_WIDTH - this->m_pGameOverText->getSize().x) / 2 - BLOCK_SIZE,
		WINDOW_HEIGHT + BLOCK_SIZE * 4
	);
}


} /* !namespace game */
