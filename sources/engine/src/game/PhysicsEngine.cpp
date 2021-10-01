#include "PhysicsEngine.hpp"

#include "game/objects/DynamicObject.hpp"
#include "game/objects/BrickWall.hpp"
#include "game/states/Level.hpp"

#include <utility>
#include <typeinfo>

namespace game {

//----------------------------------------------------------------------------//
PhysicsEngine::PhysicsEngine(Level* parent_level)
	: m_pParentLevel{ parent_level }
{}

//----------------------------------------------------------------------------//
void PhysicsEngine::addDynamicObject(DynamicObject* pGame_object)
{
	m_pDynamicObjects.push_back(pGame_object);
}

//----------------------------------------------------------------------------//
const std::vector<DynamicObject*>& PhysicsEngine::getDynamicObjects() const
{
	return this->m_pDynamicObjects;
}

//----------------------------------------------------------------------------//
void PhysicsEngine::update()
{
	for (auto itrCurrentDynamicObject{ m_pDynamicObjects.begin() };
		 itrCurrentDynamicObject != m_pDynamicObjects.end();
		 ++itrCurrentDynamicObject)
	{
		auto pCurrentDynamicObject{ *itrCurrentDynamicObject };

		if (pCurrentDynamicObject->isDisabled())
		{ continue; }

		pCurrentDynamicObject->update();
		pCurrentDynamicObject->setCollision(
			resolveStaticCollision(pCurrentDynamicObject)
		);
		resolveDynamicCollision(itrCurrentDynamicObject);
	}
}

//----------------------------------------------------------------------------//
bool PhysicsEngine::resolveStaticCollision(DynamicObject* pDynamic_object)
{
	bool hasCollision{};

	const auto dynamicObjectBounds { pDynamic_object->getGlobalBounds() };
	const auto staticObjectsToCheck{
		m_pParentLevel->getCollisionObjects(dynamicObjectBounds)
	};

	for (StaticObject* currentObjectToCheck : staticObjectsToCheck)
	{
		if (currentObjectToCheck->collides(pDynamic_object) == false)
		{ continue; }

		const auto staticObjectBounds{
			pDynamic_object->isBullet() ? currentObjectToCheck->getBoundsForBullet() :
				                          currentObjectToCheck->getGlobalBounds()
		};

		if (dynamicObjectBounds.intersects(staticObjectBounds))
		{
			hasCollision = true;
			pDynamic_object->onCollision(currentObjectToCheck);
			currentObjectToCheck->onCollision(pDynamic_object);
		}
	}

	return hasCollision;
}

//----------------------------------------------------------------------------//
bool PhysicsEngine::resolveDynamicCollision(
	std::vector<DynamicObject*>::iterator itrDynamic_object
)
{
	bool hasCollision{};

	for (auto itrObject2{ std::next(itrDynamic_object) };
		itrObject2 != m_pDynamicObjects.end();
		++itrObject2)
	{
		auto pObject1{ *itrDynamic_object };
		auto pObject2{ *itrObject2 };

		if (pObject2->isDisabled() || pObject1->getID() == pObject2->getID())
		{ continue; }

		if (pObject1->getGlobalBounds().intersects((pObject2)->getGlobalBounds()))
		{
			hasCollision = true;
			if (pObject1->isBullet())
			{
				pObject2->onCollision(pObject1);
				pObject1->onCollision(pObject2);
			}
			else
			{
				pObject1->onCollision(pObject2);
				pObject2->onCollision(pObject1);
			}
		}
	}

	return hasCollision;
}


} /* !namespace game */
