#include "ConcreteWall.hpp"
#include "Bullet.hpp"
#include "renderer/Sprite2D.hpp"
#include "game/ResourcesManager.hpp"


//----------------------------------------------------------------------------//
game::ConcreteWall::ConcreteWall(const tps::Vec2i& position)
	: StaticObject(position, tps::Vec2i{ BLOCK_SIZE, BLOCK_SIZE })
	, m_pSprite{ ResourcesManager::getSprite2D("concreteWall") }
{
	this->m_pSprite->setSize(this->getSize());
}

//----------------------------------------------------------------------------//
void game::ConcreteWall::onCollision(DynamicObject* object)
{
	const Bullet* pBullet{ dynamic_cast<const Bullet*>(object) };
	if (pBullet != nullptr && pBullet->getType() == EBulletType::Reinforced)
	{
		this->m_eObjectState = EObjectState::Destroyed;
	}
}

//----------------------------------------------------------------------------//
void game::ConcreteWall::render() const
{
	if (this->m_eObjectState == EObjectState::Enabled)
	{
		this->m_pSprite->render(this->getPosition());
	}
}
