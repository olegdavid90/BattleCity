#include "Tree.hpp"
#include "renderer/Sprite2D.hpp"
#include "game/ResourcesManager.hpp"


//----------------------------------------------------------------------------//
game::Tree::Tree(const tps::Vec2i& position)
	: StaticObject(position, tps::Vec2i{ BLOCK_SIZE, BLOCK_SIZE })
	, m_pSprite{ ResourcesManager::getSprite2D("trees") }
{
	this->m_pSprite->setSize(this->getSize());
}

//----------------------------------------------------------------------------//
void game::Tree::render() const
{
	this->m_pSprite->render(this->getPosition());
}
