#include "EnemyTank.hpp"
#include "AIComponent.hpp"
#include "game/objects/Enemies.hpp"
#include "game/objects/Bullet.hpp"
#include "game/states/Level.hpp"
#include "system/utils.hpp"
#include "game/ResourcesManager.hpp"
#include "system/log.hpp"
#include "renderer/Sprite2D.hpp"

#include <algorithm>

namespace game {

//----------------------------------------------------------------------------//
EnemyTank::EnemyTank(ESubType subtype, Enemies* owner)
	: Tank(ETankType::Enemy, ENEMY_RESPAWN_POSITIONS[utils::rand()%3])
	, m_pBonusTankSprites  { nullptr }
	, m_pBullet            { std::make_unique<Bullet>(this) }
	, m_Command            { &AIComponent::randomMoveCommand }
	, m_eSubType           { subtype }
	, m_Owner              { owner }
	, m_SpeedMask          { (subtype == ESubType::HighSpeed) ? ~0x0U : 0x1U }
	, m_isBonusTankShowTime{ false }
	, m_hasBonus           { false }
{
	if (this->m_eSubType == ESubType::Armored)
	{
		this->m_Health = 3;
	}

	this->m_isMoving = true;
	this->m_RespawnTimer.setCallback(
		[this]()
		{
			this->m_eTankState = ETankState::Active;
			this->setOrientation(EDirection::Bottom);
			this->m_TankAnimator.reset(
				this->m_pTankSprites[static_cast<unsigned>(this->getOrientation())]
			);
		}
	);
}

//----------------------------------------------------------------------------//
unsigned EnemyTank::getSubType() const
{
	return static_cast<unsigned>(this->m_eSubType);
}

//----------------------------------------------------------------------------//
void EnemyTank::render() const
{
	if (this->isDisabled())
	{ return; }

	if (this->isRespawning())
	{
		this->m_pRespawnSprite->render(
			this->getPosition(), this->m_TankAnimator.getCurrentTile()
		);
		return;
	}
	if (this->m_isBonusTankShowTime)
	{
		this->m_pBonusTankSprites[static_cast<unsigned>(this->getOrientation())]->render(
			this->m_Position, this->m_TankAnimator.getCurrentTile()
		);
	}
	else
	{
		this->m_pTankSprites[static_cast<unsigned>(this->getOrientation())]->render(
			this->m_Position, this->m_TankAnimator.getCurrentTile()
		);
	}

	this->m_pBullet->render();
}

//----------------------------------------------------------------------------//
void EnemyTank::fire()
{
	this->m_pBullet->fire();
}

//----------------------------------------------------------------------------//
void EnemyTank::update()
{
	if (this->isDisabled())
	{ return; }

	if (this->isRespawning())
	{
		this->m_TankAnimator.update();
		this->m_RespawnTimer.update();
		return;
	}
	if (this->m_hasBonus)
	{
		this->m_BonusTankShowTimer.update();
	}

	if (this->m_isFrozen)
	{ return; }

	//if (utils::rand() % 32 == 0) // FIXME
	//{ this->fire(); }

	this->m_Command(this);

	++this->m_FramesCount;
	if (this->m_FramesCount & m_SpeedMask)
	{
		this->setPosition(this->getPosition() + this->getDirection());
		this->m_TankAnimator.update();
	}
}

//----------------------------------------------------------------------------//
void EnemyTank::loadSprites()
{
    for (unsigned orientation{}; orientation < this->m_pTankSprites.size(); ++orientation)
    {
		const int currentHealth{ this->m_hasBonus ? 0 : this->getHealth() };
        std::string spriteName{
            "enemy_" + std::to_string(static_cast<unsigned>(this->getSubType()))
            + std::to_string(orientation)
			+ std::to_string(currentHealth)
        };

        this->m_pTankSprites[orientation] = ResourcesManager::getSprite2D(spriteName);
        this->m_pTankSprites[orientation]->setSize(this->getSize());

		if (this->m_hasBonus)
		{
			this->m_pBonusTankSprites[orientation]
				= ResourcesManager::getSprite2D(spriteName + "_bonus");
			this->m_pBonusTankSprites[orientation]->setSize(this->getSize());
		}
    }
}

//----------------------------------------------------------------------------//
void EnemyTank::destroy(bool desyroyed_with_bullet)
{
	if (this->m_eTankState != ETankState::Active)
	{ return; }

	if (desyroyed_with_bullet == false)
	{
		this->m_pParentLevel->setExplosion(this->getPosition());
		this->m_eTankState = ETankState::Destroyed;
		this->m_Owner->removeTank();
		return;
	}

	if (this->m_hasBonus)
	{
		this->m_hasBonus = false;
		this->m_isBonusTankShowTime = false;
		this->m_pParentLevel->createBonus();
	}

	if (--this->m_Health >= 0)
	{
		this->loadSprites();
	}
	else
	{
		const unsigned score{
			(static_cast<unsigned>(this->getSubType()) + 1) * 100
		};
		this->m_pParentLevel->setExplosion(this->getPosition(), score);
		this->m_eTankState = ETankState::Destroyed;
		this->m_Owner->removeTank();
	}
}

//----------------------------------------------------------------------------//
void EnemyTank::setLevel(Level* parent_level)
{
	this->m_pParentLevel = parent_level;
	this->loadSprites();
	this->m_pParentLevel->addActiveDynamicObject(this->m_pBullet.get());
	this->m_RespawnTimer.start(1);
}

//----------------------------------------------------------------------------//
void EnemyTank::setBonus()
{
	this->m_hasBonus = true;
	this->m_BonusTankShowTimer.setCallback(
		[this]()
		{
			this->m_isBonusTankShowTime = !this->m_isBonusTankShowTime;
			this->m_BonusTankShowTimer.start(8);
		}
	);
	this->m_BonusTankShowTimer.start(8);
}

//----------------------------------------------------------------------------//
void EnemyTank::changeScenario()
{
	switch (this->m_Owner->getActionType())
	{
	case EActionType::RandomMove:
		this->setRandomDirection();
		this->setCommand(&AIComponent::randomMoveCommand);
		break;
	case EActionType::MoveToPlayer:
		this->setCommand(&AIComponent::moveToPlayerCommand);
		break;
	case EActionType::MoveToEagle:
		this->setCommand(&AIComponent::moveToEagleCommand);
	}
}

//----------------------------------------------------------------------------//
void EnemyTank::invertDirection()
{
	switch (this->getOrientation())
	{
	case EDirection::Top:
		this->setOrientation(EOrientation::Bottom);
		return;
	case EDirection::Bottom:
		this->setOrientation(EOrientation::Top);
		return;
	case EDirection::Left:
		this->setOrientation(EOrientation::Right);
		return;
	case EDirection::Right:
		this->setOrientation(EOrientation::Left);
		return;
	default:
		break;
	}
}

//----------------------------------------------------------------------------//
void EnemyTank::setRandomDirection()
{
	this->setOrientation(
		static_cast<EOrientation>(utils::rand() % 4)
	);
}

//----------------------------------------------------------------------------//
void EnemyTank::rotateClockwise()
{
	switch (this->getOrientation())
	{
	case EDirection::Top:
		this->setOrientation(EOrientation::Left);
		return;
	case EDirection::Bottom:
		this->setOrientation(EOrientation::Right);
		return;
	case EDirection::Left:
		this->setOrientation(EOrientation::Top);
		return;
	case EDirection::Right:
		this->setOrientation(EOrientation::Bottom);
		return;
	default:
		break;
	}
}

//----------------------------------------------------------------------------//
void EnemyTank::rotateAntiClockwise()
{
	switch (this->getOrientation())
	{
	case EDirection::Top:
		this->setOrientation(EOrientation::Right);
		return;
	case EDirection::Bottom:
		this->setOrientation(EOrientation::Left);
		return;
	case EDirection::Left:
		this->setOrientation(EOrientation::Bottom);
		return;
	case EDirection::Right:
		this->setOrientation(EOrientation::Top);
		return;
	default:
		break;
	}
}

//----------------------------------------------------------------------------//
void EnemyTank::setCommand(const Command& command)
{
	this->m_Command = command;
}


} /* !namespace game */
