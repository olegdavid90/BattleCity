#include "Border.hpp"
#include "renderer/Sprite2D.hpp"
#include "game/ResourcesManager.hpp"


//----------------------------------------------------------------------------//
game::Border::Border(const tps::Vec2i& position, const tps::Vec2i& size)
	: StaticObject(position, size)
	, m_pSprite{ ResourcesManager::getSprite2D("border") }
{
	this->m_pSprite->setSize(this->getSize());
}

//----------------------------------------------------------------------------//
void game::Border::render() const
{
	this->m_pSprite->render(this->getPosition(), this->getSize());
}
