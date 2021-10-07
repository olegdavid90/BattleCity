#include "PlayerTank.hpp"
#include "system/log.hpp"
#include "game/states/Level.hpp"
#include "game/objects/Ice.hpp"
#include "game/objects/Bullet.hpp"
#include "game/InputHandler.hpp"
#include "game/ResourcesManager.hpp"
#include "renderer/Sprite2D.hpp"

#include <algorithm>


//----------------------------------------------------------------------------//
game::PlayerTank::PlayerTank()
	: Tank(ETankType::Player, PLAYER_RESPAWN_POSITION)
	, m_pShieldSprite { ResourcesManager::getSprite2D("shield")}
	, m_ShieldAnimator{ this->m_pShieldSprite }
	, m_eSubType      { ESubType::Small }
	, m_Score         { 0 }
	, m_BulletsCount  { 1 }
	, m_BulletIndex   { 0 }
	, m_hasShield     { false }
	, m_isCooldownTime{ false }
	, m_onIce         { false }
	, m_isSlipping    { false }
{
	this->m_Health = 2;
	this->m_pShieldSprite->setSize(this->getSize());
	this->initTimers();

	for (auto& pBullet : this->m_pBullets)
	{
		pBullet = std::make_unique<Bullet>(this);
	}
}

//----------------------------------------------------------------------------//
bool game::PlayerTank::hasShield() const
{
	return this->m_hasShield;
}

//----------------------------------------------------------------------------//
void game::PlayerTank::render() const
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

	this->m_pTankSprites[static_cast<unsigned>(this->getOrientation())]->render(
		this->m_Position, this->m_TankAnimator.getCurrentTile()
	);
	for (unsigned noBullet{}; noBullet < this->m_BulletsCount; ++noBullet)
	{
		this->m_pBullets[noBullet]->render();
	}

	if (this->m_hasShield)
	{
		this->m_pShieldSprite->render(
			this->m_Position, m_ShieldAnimator.getCurrentTile()
		);
	}
}

//----------------------------------------------------------------------------//
void game::PlayerTank::update()
{
	if (this->m_eTankState == ETankState::Respawn)
	{
		this->m_TankAnimator.update();
		this->m_RespawnTimer.update();
		return;
	}

	if (this->m_hasShield)
	{
		this->m_ShieldAnimator.update();
		this->m_ShieldTimer.update();
	}


	if (this->isMoving() || this->m_isSlipping)
	{
		++this->m_FramesCount;
		this->m_FramesCount &= 0x3;

		if (this->m_FramesCount != 0)
		{
			this->setPosition(this->getPosition() + this->getDirection());
			if (this->isMoving())
			{
				this->m_TankAnimator.update();
			}
			this->m_SlippingTimer.update();
		}
	}
	if (this->m_hasCollision == false)
	{
		this->stop();
		this->m_onIce = false;
	}
	this->m_CooldownTimer.update();
}

//----------------------------------------------------------------------------//
void game::PlayerTank::Reset()
{
	this->m_eTankState = ETankState::Respawn;
	this->m_eSubType   = ESubType::Small;
	this->setPosition(PLAYER_RESPAWN_POSITION);
	this->m_oldPosition = PLAYER_RESPAWN_POSITION;
	this->loadSprites();
	this->m_RespawnTimer.start(1);
}

//----------------------------------------------------------------------------//
void game::PlayerTank::initTimers()
{
	this->m_ShieldTimer.setCallback([this] { this->m_hasShield = false; });
	this->m_CooldownTimer.setCallback([this] { this->m_isCooldownTime = true; });
	this->m_RespawnTimer.setCallback(
		[this]()
		{
			this->setShield(2);
			this->m_eTankState = ETankState::Active;
			this->setOrientation(EDirection::Top);
			this->m_TankAnimator.reset(
				this->m_pTankSprites[static_cast<unsigned>(this->getOrientation())]
			);
		}
	);
	this->m_SlippingTimer.setCallback(
		[this]()
		{
			this->m_isSlipping = false;
			this->stop();
		}
	);
}

//----------------------------------------------------------------------------//
void game::PlayerTank::destroy(bool)
{
	this->m_pParentLevel->setExplosion(this->getPosition());

	if (--this->m_Health >= 0)
	{
		this->Reset();
	}
	else
	{
		this->m_eTankState = ETankState::Destroyed;
		this->m_pParentLevel->startGameOver();
	}
}

//----------------------------------------------------------------------------//
void game::PlayerTank::setShield(unsigned seconds)
{
	this->m_hasShield = true;
	this->m_ShieldTimer.start(seconds);
}

//----------------------------------------------------------------------------//
void game::PlayerTank::addStar()
{
	this->m_eSubType = static_cast<ESubType>(
		std::min(static_cast<unsigned>(this->m_eSubType) + 1, 3U)
	);
	this->loadSprites();
}

//----------------------------------------------------------------------------//
void game::PlayerTank::updateInput()
{
	static bool keys[6];
	InputHandler::update(keys);

	if (keys[KEY_J])
	{ this->fire(); }
    
	if (keys[KEY_W])
	{
		this->setOrientation(Tank::EOrientation::Top);
	}
	else if (keys[KEY_S])
	{
		this->setOrientation(Tank::EOrientation::Bottom);
	}
	else if (keys[KEY_A])
	{
		this->setOrientation(Tank::EOrientation::Left);
	}
	else if (keys[KEY_D])
	{
		this->setOrientation(Tank::EOrientation::Right);
	}
	else if (this->isMoving())
	{
		this->stop();
		if (this->m_onIce)
		{
			this->m_isSlipping = true;
			this->m_SlippingTimer.start(21);
		}
	}
}

//----------------------------------------------------------------------------//
void game::PlayerTank::setScore(unsigned score)
{
	this->m_Score = score;
}

//----------------------------------------------------------------------------//
void game::PlayerTank::setHealth(unsigned health)
{
	this->m_Health = health;
}

//----------------------------------------------------------------------------//
void game::PlayerTank::setSubType(unsigned subType)
{
	this->m_eSubType = static_cast<ESubType>(subType);
}

//----------------------------------------------------------------------------//
unsigned game::PlayerTank::getScore() const
{
	return this->m_Score;
}

//----------------------------------------------------------------------------//
void game::PlayerTank::setLevel(Level* parent_level)
{
	this->m_pParentLevel = parent_level;
	this->m_eTankState  = ETankState::Respawn;
	this->loadSprites();
	this->setPosition(PLAYER_RESPAWN_POSITION);

	for (auto& pBullet : this->m_pBullets)
	{
		pBullet->ResetState();
		this->m_pParentLevel->addActiveDynamicObject(pBullet.get());
	}

	this->m_RespawnTimer.start(1);
}

//----------------------------------------------------------------------------//
void game::PlayerTank::onCollision(const StaticObject* object)
{
	if (typeid(*object) != typeid(Ice))
	{
		Tank::onCollision(object);
		this->m_isSlipping = false;
	}
	else
	{
		this->m_onIce = true;
	}
}

//----------------------------------------------------------------------------//
void game::PlayerTank::setOrientation(EOrientation orientation)
{
	this->start();

	if (this->m_isSlipping == false)
	{
		Tank::setOrientation(orientation);
	}
}

//----------------------------------------------------------------------------//
unsigned game::PlayerTank::getSubType() const
{
	return static_cast<unsigned>(this->m_eSubType);
}

//----------------------------------------------------------------------------//
void game::PlayerTank::loadSprites()
{
	for (unsigned orientation{}; orientation < this->m_pTankSprites.size(); ++orientation)
	{
		std::string spriteName{
			"player_"
			+ std::to_string(static_cast<unsigned>(this->getSubType()))
			+ std::to_string(orientation)
		};
		this->m_pTankSprites[orientation] = ResourcesManager::getSprite2D(spriteName);
		this->m_pTankSprites[orientation]->setSize(this->getSize());
	}
}

//----------------------------------------------------------------------------//
void game::PlayerTank::fire()
{
	if (this->m_eTankState != ETankState::Active)
	{ return; }

	this->m_BulletsCount = static_cast<unsigned>(this->m_eSubType) / 2 + 1;

	if (this->m_isCooldownTime == true)
	{
		if (++this->m_BulletIndex >= this->m_BulletsCount)
		{
			this->m_BulletIndex = 0;
		}
	}
	if (this->m_pBullets[this->m_BulletIndex]->fire() == true)
	{
		this->m_isCooldownTime = false;
		this->m_CooldownTimer.start(8);
	}
}

//----------------------------------------------------------------------------//
void game::PlayerTank::stop()
{
	DynamicObject::stop();
	this->m_isSlipping = false;
}
