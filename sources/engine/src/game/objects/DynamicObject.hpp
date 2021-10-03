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

	virtual void onCollision(const class StaticObject*) = 0;
	virtual void onCollisionWithBullet(class Bullet*)   = 0;
	virtual void onCollisionWithTank(class Tank*)       = 0;
	virtual bool isPlayer()     const                   = 0;
	virtual std::size_t getID() const                   = 0;

	void onCollision(DynamicObject*) override;

	virtual void setOrientation(EOrientation orientation);
	virtual void stop();
	virtual bool isTank()            const;
	virtual bool isBullet()          const;
	virtual bool isMoving()          const;

	void start();
	EOrientation getOrientation()    const;
	const tps::Vec2i& getDirection() const;
	void setCollision(bool is_collision);
	void setDirection(const tps::Vec2i& direction);


  protected:
	EOrientation  m_eOrientation{ EOrientation::Top };
	tps::Vec2i    m_Direction   { 0, -1 };
	bool          m_isMoving    { false };
	bool          m_hasCollision{ false };

}; /* !class DynamicObject */


} /* !namespace game */
