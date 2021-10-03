#pragma once

#include "GameObject.hpp"
#include "system/EDirection.hpp"

#include <cstddef>
#include <vector>

namespace game {


class StaticObject : public GameObject
{
  public:
	using GameObject::GameObject;
	virtual ~StaticObject() = default;


	virtual void onCollision(class DynamicObject*) {}
	virtual bool collides(const class DynamicObject*) const;
	virtual tps::IntRect getBoundsForBullet()         const;

}; /* !class StaticObject */


} /* !namespace game */

