#pragma once

#include "GameObject.hpp"
#include "system/EDirection.hpp"

#include <cstddef>

namespace game {


class DynamicObject : public GameObject
{
  public:
	typedef EDirection EOrientation;

  public:
	using GameObject::GameObject;
	virtual ~DynamicObject() = default;

	virtual void onCollision(const class StaticObject*)     = 0;
	virtual bool isPlayer()     const                       = 0;
	virtual std::size_t getID() const                       = 0;
	virtual void onCollisionWithBullet(const class Bullet*) = 0;
	virtual void onCollisionWithTank(const class Tank*)     = 0;

	virtual void onCollision(const DynamicObject*);
	virtual void setOrientation(EOrientation orientation);
	virtual void setDirection(const tps::Vec2i& direction);
	virtual void start();
	virtual void stop();
	virtual bool isTank()                    const;
	virtual bool isBullet()                  const;
	virtual bool isMoving()                  const;
	virtual EOrientation getOrientation()    const;
	virtual tps::IntRect getGlobalBounds()   const;
	virtual const tps::Vec2i& getDirection() const;


  protected:
	EOrientation  m_eOrientation{ EOrientation::Top };
	tps::Vec2i    m_Direction   { 0, -1 };
	bool          m_isMoving    { false };

}; /* !class DynamicObject */


} /* !namespace game */
