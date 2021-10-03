#include "GameObject.hpp"

namespace game {

//----------------------------------------------------------------------------//
GameObject::GameObject(const tps::Vec2i& position, const tps::Vec2i& size)
	: m_Position    { position }
	, m_Size        { size }
	, m_eObjectState{ EObjectState::Enabled }
{ }

//----------------------------------------------------------------------------//
tps::IntRect GameObject::getGlobalBounds() const
{
	if (this->isDisabled())
	{
		return tps::IntRect{};
	}

	return tps::IntRect{ this->getPosition(), this->getSize() };
}

//----------------------------------------------------------------------------//
const tps::Vec2i& GameObject::getPosition() const
{
	return this->m_Position;
}

//----------------------------------------------------------------------------//
const tps::Vec2i& GameObject::getSize() const
{
	return this->m_Size;
}

//----------------------------------------------------------------------------//
void GameObject::update()
{ }

//----------------------------------------------------------------------------//
bool GameObject::isDisabled() const
{
	return (this->m_eObjectState == EObjectState::Disabled);
}

//----------------------------------------------------------------------------//
void GameObject::setPosition(const tps::Vec2i& position)
{
	this->m_Position = position;
}

//----------------------------------------------------------------------------//
void GameObject::setPosition(int x, int y)
{
	this->setPosition(tps::Vec2i{ x, y });

}

//----------------------------------------------------------------------------//
void GameObject::setSize(tps::Vec2i size)
{
	this->m_Size = size;
}

//----------------------------------------------------------------------------//
void GameObject::setSize(int width, int height)
{
	this->setSize(tps::Vec2i{ width, height });
}

} /* !namespace game */
