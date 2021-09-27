#include "Water.hpp"
#include "DynamicObject.hpp"
#include "renderer/Sprite2D.hpp"
#include "renderer/Animator.hpp"
#include "game/ResourcesManager.hpp"

namespace game {

//----------------------------------------------------------------------------//
Water::Water(const tps::Vec2i& position)
	: StaticObject(position, tps::Vec2i{ BLOCK_SIZE, BLOCK_SIZE })
	, m_pSprite { ResourcesManager::getSprite2D("water") }
	, m_Animator{ this->m_pSprite }
{
	this->m_pSprite->setSize(this->getSize());
}

//----------------------------------------------------------------------------//
void Water::render() const
{
	this->m_pSprite->render(this->getPosition(), this->m_Animator.getCurrentTile());
}

//----------------------------------------------------------------------------//
void Water::update()
{
	this->m_Animator.update();
}

//----------------------------------------------------------------------------//
bool Water::collides(const DynamicObject* object) const
{
	return (object->isTank());
}


} /* !namespace game */
