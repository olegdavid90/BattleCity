#include "Tank.hpp"
#include "system/log.hpp"
#include "system/utils.hpp"
#include "game/states/Level.hpp"
#include "game/objects/Bullet.hpp"
#include "game/ResourcesManager.hpp"
#include "renderer/Sprite2D.hpp"

#include <algorithm>
#include <typeinfo>

namespace game {

//----------------------------------------------------------------------------//
Tank::Tank(ETankType type, const tps::Vec2i& position)
	: DynamicObject(position, tps::Vec2i{ BLOCK_SIZE * 2, BLOCK_SIZE * 2 })
	, m_pTankSprites  { nullptr }
	, m_pRespawnSprite{ ResourcesManager::getSprite2D("respawn") }
	, m_TankAnimator  { this->m_pRespawnSprite }
	, m_pParentLevel  { nullptr }
	, m_eTankType     { type }
	, m_eTankState    { ETankState::Respawn }
	, m_oldPosition   { position }
	, m_FramesCount   { 0 }
	, m_Health        { 0 }
	, m_isFrozen      { false }
{
	this->m_pRespawnSprite->setSize(this->getSize());
}

//----------------------------------------------------------------------------//
bool Tank::hasShield() const
{
	return false;
}

//----------------------------------------------------------------------------//
int Tank::getHealth() const
{
	return this->m_Health;
}

//----------------------------------------------------------------------------//
void Tank::setPosition(const tps::Vec2i& position)
{
	this->m_oldPosition = this->getPosition();
	DynamicObject::setPosition(position);
}

//----------------------------------------------------------------------------//
void Tank::setOrientation(EOrientation orientation)
{
	if (this->isRespawning() || this->getOrientation() == orientation)
	{ return; }

	DynamicObject::setOrientation(orientation);
	this->alignmentPosition();
}

//----------------------------------------------------------------------------//
bool Tank::isTank() const
{
	return true;
}

//----------------------------------------------------------------------------//
bool Tank::isPlayer() const
{
	return (this->m_eTankType == ETankType::Player);
}

//----------------------------------------------------------------------------//
const Level* Tank::getParentLevel() const
{
	return this->m_pParentLevel;
}

//----------------------------------------------------------------------------//
bool Tank::isRespawning() const
{
	return this->m_eTankState == ETankState::Respawn;
}

//----------------------------------------------------------------------------//
bool Tank::frontTileIsBarrier() const
{
	return this->m_hasCollision;
}

//----------------------------------------------------------------------------//
void Tank::freeze(bool is_frozen)
{
	this->m_isFrozen = is_frozen;
}

//----------------------------------------------------------------------------//
void Tank::onCollision(const StaticObject* object)
{
	DynamicObject::setPosition(this->m_oldPosition);
}

//----------------------------------------------------------------------------//
void Tank::onCollisionWithBullet(const Bullet* bullet)
{
	if (this->isRespawning() || this->hasShield() || bullet->isExplosion())
	{ return; }

	if (bullet->isPlayer() != this->isPlayer())
	{
		this->destroy(true);
	}
}

//----------------------------------------------------------------------------//
void Tank::onCollisionWithTank(const Tank* other)
{
	if (this->isRespawning() || this->m_isFrozen)
	{ return; }

	const tps::Vec2i otherTankPosition{ other->getPosition() };

	if (this->isPlayer() == false)
	{
		switch (this->getOrientation())
		{
		case EOrientation::Top:
			if (this->getPosition().y > otherTankPosition.y)
			{
				this->setOrientation(EOrientation::Bottom);
			}
			return;
		case EOrientation::Bottom:
			if (this->getPosition().y < otherTankPosition.y)
			{
				this->setOrientation(EOrientation::Top);
			}
			return;
		case EOrientation::Left:
			if (this->getPosition().x > otherTankPosition.x)
			{
				this->setOrientation(EOrientation::Right);
			}
			return;
		case EOrientation::Right:
			if (this->getPosition().x < otherTankPosition.x)
			{
				this->setOrientation(EOrientation::Left);
			}
			return;
		default: return;
		}
	}

	this->stop();
	DynamicObject::setPosition(this->m_oldPosition);
}

//----------------------------------------------------------------------------//
void Tank::alignmentPosition()
{
	if (this->getDirection().x != 0)
	{
		this->m_Position.y =
			(this->getPosition().y + BLOCK_SIZE / 2) / BLOCK_SIZE * BLOCK_SIZE;
	}
	else
	{
		this->m_Position.x =
			(this->getPosition().x + BLOCK_SIZE / 2) / BLOCK_SIZE * BLOCK_SIZE;
	}
}

//----------------------------------------------------------------------------//
bool Tank::isDisabled() const
{
	return (this->m_eTankState == ETankState::Destroyed);
}

//----------------------------------------------------------------------------//
std::size_t Tank::getID() const
{
	return (std::size_t)this;
}


} /* !namespace game */
