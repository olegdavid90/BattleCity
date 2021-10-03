#include "Eagle.hpp"
#include "DynamicObject.hpp"
#include "renderer/Sprite2D.hpp"
#include "game/states/Level.hpp"
#include "game/ResourcesManager.hpp"


//----------------------------------------------------------------------------//
game::Eagle::Eagle(class Level* parent_level)
	: StaticObject(EAGLE_POSITION, { BLOCK_SIZE * 2, BLOCK_SIZE * 2 })
	, m_pSprite     { ResourcesManager::getSprite2D("eagle") }
	, m_pParentLevel{ parent_level }
{
	this->m_pSprite->setSize(this->getSize());
}

//----------------------------------------------------------------------------//
void game::Eagle::render() const
{
	this->m_pSprite->render(this->getPosition(), this->isDisabled() ? 1U : 0U);
}

//----------------------------------------------------------------------------//
void game::Eagle::onCollision(DynamicObject* object)
{
	if (object->isBullet() == false || object->isPlayer() || this->isDisabled())
	{ return; }

	this->m_eObjectState = EObjectState::Disabled;
	this->m_pParentLevel->setExplosion(this->getPosition());
	this->m_pParentLevel->startGameOver();

}
