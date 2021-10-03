#pragma once

#include "system/types.hpp"
#include "system/EDirection.hpp"
#include "system/SecondsTimer.hpp"

#include <functional>
#include <vector>
#include <memory>


namespace game {

class DynamicObject;
class Level;


class PhysicsEngine
{
  public:
	PhysicsEngine(class Level* parent_level);

	void update();
	void addDynamicObject(DynamicObject* pGame_object);
	const std::vector<DynamicObject*>& getDynamicObjects() const;

  private:
	bool resolveStaticCollision(DynamicObject* pDynamic_object);
	void resolveDynamicCollision(
		std::vector<DynamicObject*>::iterator itrDynamic_object
	);


  private:
	std::vector<DynamicObject*>  m_pDynamicObjects;
	const Level*                 m_pParentLevel;

}; /* !class Engine */


} /* !namespace game */
