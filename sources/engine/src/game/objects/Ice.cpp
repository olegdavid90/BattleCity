#include "Ice.hpp"
#include "DynamicObject.hpp"
#include "renderer/Sprite2D.hpp"
#include "game/ResourcesManager.hpp"


//----------------------------------------------------------------------------//
game::Ice::Ice(const tps::Vec2i& position)
	: StaticObject(position, tps::Vec2i{ BLOCK_SIZE, BLOCK_SIZE })
	, m_pSprite{ ResourcesManager::getSprite2D("ice") }
{
	this->m_pSprite->setSize(this->getSize());
}

//----------------------------------------------------------------------------//
void game::Ice::render() const
{
	this->m_pSprite->render(this->getPosition());
}

//----------------------------------------------------------------------------//
bool game::Ice::collides(const DynamicObject* object) const
{
	return (object->isTank() && object->isPlayer());
}

