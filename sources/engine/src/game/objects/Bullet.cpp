#include "Bullet.hpp"
#include "StaticObject.hpp"
#include "system/log.hpp"
#include "game/objects/tanks/Tank.hpp"
#include "game/objects/tanks/EnemyTank.hpp"
#include "game/objects/tanks/PlayerTank.hpp"
#include "game/ResourcesManager.hpp"
#include "system/EDirection.hpp"
#include "renderer/Sprite2D.hpp"


namespace game {

//----------------------------------------------------------------------------//
Bullet::Bullet(Tank* owner)
	: DynamicObject(tps::Vec2i{}, tps::Vec2i{ BLOCK_SIZE, BLOCK_SIZE })
	, m_pBulletSprites   {
		ResourcesManager::getSprite2D("bullet_top"),
		ResourcesManager::getSprite2D("bullet_bottom"),
		ResourcesManager::getSprite2D("bullet_left"),
		ResourcesManager::getSprite2D("bullet_right")
	}
	, m_pExplosionSprite { ResourcesManager::getSprite2D("explosion") }
	, m_ExplosionAnimator{ this->m_pExplosionSprite }
	, m_ExplosionOffset  { tps::Vec2i{ -BLOCK_SIZE / 2, -BLOCK_SIZE / 2 } }
	, m_Owner            { owner }
	, m_eBulletType      { EBulletType::Slow }
	, m_eBulletState     { EBulletState::Wait }
{
	for (auto currentSprite : this->m_pBulletSprites)
	{
		currentSprite->setSize(this->getSize());
	}

	this->m_pExplosionSprite->setSize(this->getSize() * 2);
    this->updateType();

	this->m_ExplosionTimer.setCallback([this] { this->ResetState(); });
}

//----------------------------------------------------------------------------//
bool Bullet::isPlayer() const
{
	return this->m_Owner->isPlayer();
}

//----------------------------------------------------------------------------//
bool Bullet::isDisabled() const
{
	return (this->m_eBulletState == EBulletState::Wait);
}

//----------------------------------------------------------------------------//
std::size_t Bullet::getID() const
{
	return this->m_Owner->getID();
}


//----------------------------------------------------------------------------//
void Bullet::update()
{
	if (this->isExplosion())
	{
		this->m_ExplosionAnimator.update();
		this->m_ExplosionTimer.update();
		return;
	}
	if (this->isMoving())
	{
		this->setPosition(
			this->getPosition() + this->getDirection() * this->m_Speed
		);
	}
}

//----------------------------------------------------------------------------//
void Bullet::updateType()
{
    if (this->isPlayer())
    {
        switch (static_cast<PlayerTank::ESubType>(this->m_Owner->getSubType()))
        {
        case PlayerTank::ESubType::Small:
            this->m_eBulletType = EBulletType::Slow;
            break;
        case PlayerTank::ESubType::Heavy:
            this->m_eBulletType = EBulletType::Reinforced;
            break;
        default:
            this->m_eBulletType = EBulletType::Fast;
        }
    }
    else
    {
        switch (static_cast<EnemyTank::ESubType>(this->m_Owner->getSubType()))
        {
        case EnemyTank::ESubType::RapidFire:
            this->m_eBulletType = EBulletType::Fast;
            break;
        default:
            this->m_eBulletType = EBulletType::Slow;
        }
    }
    
	this->m_Speed = (this->m_eBulletType == EBulletType::Slow) ? 2 : 4;
}

//----------------------------------------------------------------------------//
void Bullet::ResetState()
{
	this->m_eBulletState = EBulletState::Wait;
}

//----------------------------------------------------------------------------//
void Bullet::render() const
{
	if (this->isDisabled())
	{ return; }

	if (this->isExplosion())
	{
		const tps::Vec2i currentExplosionOffset{
			this->m_ExplosionOffset + this->getDirection() * 4
		};
		this->m_pExplosionSprite->render(
			this->getPosition() + currentExplosionOffset,
			this->m_ExplosionAnimator.getCurrentTile()
		);

		return;
	}

	this->m_pBulletSprites[static_cast<unsigned>(this->getOrientation())]->render(
		this->getPosition()
	);
}

//----------------------------------------------------------------------------//
bool Bullet::isBullet() const
{
	return true;
}

//----------------------------------------------------------------------------//
void Bullet::onCollision(const StaticObject* object)
{
	if (this->isExplosion() || this->isDisabled())
	{ return; }

	this->stop();
	this->m_eBulletState = EBulletState::Explosion;
	this->m_ExplosionTimer.start(this->m_ExplosionAnimator.getTotalDuration());
}

//----------------------------------------------------------------------------//
void Bullet::onCollisionWithBullet(const Bullet*)
{
	if (this->isExplosion() || this->isDisabled())
	{ return; }

	this->stop();
	this->ResetState();
}

//----------------------------------------------------------------------------//
void Bullet::onCollisionWithTank(const Tank* tank)
{
	if (this->isExplosion() || this->isDisabled() || tank->isRespawning())
	{ return; }

	this->stop();

	if (this->isPlayer() == tank->isPlayer() || tank->hasShield())
	{
		this->ResetState();
		return;
	}

	this->m_eBulletState = EBulletState::Explosion;
	this->m_ExplosionTimer.start(this->m_ExplosionAnimator.getTotalDuration());
}

//----------------------------------------------------------------------------//
bool Bullet::isExplosion() const
{
	return 	(this->m_eBulletState == EBulletState::Explosion);
}

//----------------------------------------------------------------------------//
EBulletType Bullet::getType() const
{
	return this->m_eBulletType;
}

//----------------------------------------------------------------------------//
bool Bullet::fire()
{
	if (this->m_eBulletState != EBulletState::Wait)
	{ return false; }

	if (this->isPlayer())
	{
		this->updateType();
	}

	tps::Vec2i offsetPosition{};
	switch (this->m_Owner->getOrientation())
	{
	case EOrientation::Top:
		offsetPosition = { BLOCK_SIZE / 2, 0 };
		break;
	case EOrientation::Bottom:
		offsetPosition = { BLOCK_SIZE / 2,  BLOCK_SIZE };
		break;
	case EOrientation::Left:
		offsetPosition = { 0, BLOCK_SIZE / 2 };
		break;
	case EOrientation::Right:
		offsetPosition = { BLOCK_SIZE, BLOCK_SIZE / 2 };
	}

	this->setOrientation(this->m_Owner->getOrientation());
	this->setPosition(this->m_Owner->getPosition() + offsetPosition);
	this->m_eBulletState = EBulletState::Active;
	this->start();

	return true;
}


} /* !namespace game */
