#include "Bonus.hpp"
#include "IEngine.hpp"
#include "system/log.hpp"
#include "DynamicObject.hpp"
#include "system/types.hpp"
#include "system/utils.hpp"
#include "renderer/Sprite2D.hpp"
#include "game/states/Level.hpp"
#include "game/objects/tanks/PlayerTank.hpp"
#include "game/ResourcesManager.hpp"

#include <algorithm>


//----------------------------------------------------------------------------//
game::tps::Vec2i game::Bonus::getRandomBonusPosition()
{
	const int leftOffsetPerBlocks{
		std::clamp(utils::rand() % BLOCKS_PER_GAME_WIDTH, 2, BLOCKS_PER_GAME_WIDTH - 6)
	};
	const int topOffsetPerBlocks{
		std::clamp(utils::rand() % BLOCKS_PER_GAME_WIDTH, 1, BLOCKS_PER_GAME_WIDTH - 3)
	};

	return { leftOffsetPerBlocks * BLOCK_SIZE, topOffsetPerBlocks * BLOCK_SIZE };
}

//----------------------------------------------------------------------------//
game::Bonus::Bonus(Level* parent_level)
	: StaticObject(getRandomBonusPosition(), tps::Vec2i{ BLOCK_SIZE * 2, BLOCK_SIZE * 2})
	, m_pParentLevel{ parent_level }
	, m_isShowTime  { true }
{
	unsigned bonusIndex{ utils::rand() % 8U };
	if (bonusIndex > static_cast<unsigned>(EBonusType::Tank))
	{
		bonusIndex -= 4;
	}
	this->m_eBonusType = static_cast<EBonusType>(bonusIndex);

	this->m_pSprite =
		ResourcesManager::getSprite2D("bonus_" + std::to_string(bonusIndex));
	this->m_pSprite->setSize(this->getSize());
	this->m_ShowTimer.setCallback(
		[this]()
		{
			this->m_isShowTime = !this->m_isShowTime;
			this->m_ShowTimer.start(8);
		}
	);

	this->m_ShowTimer.start(8);
}

//----------------------------------------------------------------------------//
void game::Bonus::render() const
{
	if (this->m_isShowTime)
	{
		this->m_pSprite->render(this->getPosition());
	}
}

//----------------------------------------------------------------------------//
bool game::Bonus::collides(const DynamicObject* object) const
{
	return (object->isTank() && object->isPlayer());
}

//----------------------------------------------------------------------------//
void game::Bonus::onCollision(DynamicObject* player_tank)
{
	PlayerTank* pPlayer{ dynamic_cast<PlayerTank*>(player_tank) };

	if (pPlayer == nullptr)
	{ return; }

	switch (this->m_eBonusType)
	{
	case EBonusType::Helmet:
		pPlayer->setShield(10);
		break;
	case EBonusType::Clock:
		this->m_pParentLevel->freezeEnemies();
		break;
	case EBonusType::Shovel:
		this->m_pParentLevel->concreteWallsEagle();
		break;
	case EBonusType::Star:
		pPlayer->addStar();
		break;
	case EBonusType::Grenade:
		this->m_pParentLevel->removeActiveEnemies();
		break;
	case EBonusType::Tank:
		pPlayer->setHealth(pPlayer->getHealth() + 1);
		break;
	default: break;
	}

	pPlayer->setScore(pPlayer->getScore() + 500);
	this->m_isShowTime = true;
	this->m_eObjectState = EObjectState::Destroyed;
	this->m_pSprite = ResourcesManager::getSprite2D("score_500");
	this->m_pSprite->setSize(this->getSize() * 2);

	this->setPosition(
		this->getPosition() - tps::Vec2i{ BLOCK_SIZE, BLOCK_SIZE }
	);
	this->m_ShowTimer.setCallback(
		[this]
		{
			this->m_isShowTime = false;
		}
	);
	this->m_ShowTimer.start(16);
}

//----------------------------------------------------------------------------//
void game::Bonus::update()
{
	this->m_ShowTimer.update();
}
