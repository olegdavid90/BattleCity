#include "DynamicObject.hpp"
#include "tanks/Tank.hpp"
#include "Bullet.hpp"
#include "system/log.hpp"
#include "renderer/Sprite2D.hpp"

namespace game {

//----------------------------------------------------------------------------//
const tps::Vec2i& DynamicObject::getDirection() const
{
	return this->m_Direction;
}

//----------------------------------------------------------------------------//
void DynamicObject::start()
{
	this->m_isMoving = true;
}

//----------------------------------------------------------------------------//
void DynamicObject::stop()
{
	this->m_isMoving = false;
}

//----------------------------------------------------------------------------//
bool DynamicObject::isBullet() const
{
	return false;
}

//----------------------------------------------------------------------------//
bool DynamicObject::isTank() const
{
	return false;
}

//----------------------------------------------------------------------------//
bool DynamicObject::isMoving() const
{
	return this->m_isMoving;
}

//----------------------------------------------------------------------------//
DynamicObject::EOrientation DynamicObject::getOrientation() const
{
	return this->m_eOrientation;
}

//----------------------------------------------------------------------------//
void DynamicObject::onCollision(DynamicObject* object)
{
	if (object->isTank())
	{
		this->onCollisionWithTank(dynamic_cast<Tank*>(object));
	}
	else
	{
		this->onCollisionWithBullet(dynamic_cast<Bullet*>(object));
	}
}

//----------------------------------------------------------------------------//
void DynamicObject::setOrientation(EOrientation orientation)
{
	this->m_eOrientation = orientation;

	switch (this->m_eOrientation)
	{
	case EDirection::Top:
		this->setDirection(tps::Vec2i{ 0, -1 });
		return;
	case EDirection::Bottom:
		this->setDirection(tps::Vec2i{ 0, 1 });
		return;
	case EDirection::Left:
		this->setDirection(tps::Vec2i{ -1, 0 });
		return;
	case EDirection::Right:
		this->setDirection(tps::Vec2i{ 1, 0 });
		return;
	default: return;
	}
}

//----------------------------------------------------------------------------//
void DynamicObject::setDirection(const tps::Vec2i& direction)
{
	this->m_Direction = direction;
}

//----------------------------------------------------------------------------//
void DynamicObject::setCollision(bool is_collision)
{
	this->m_hasCollision = is_collision;
}


} /* !namespace game */
