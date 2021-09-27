#include "StaticObject.hpp"


//----------------------------------------------------------------------------//
bool game::StaticObject::collides(const DynamicObject*) const
{
	return true;
}

//----------------------------------------------------------------------------//
const game::tps::IntRect& game::StaticObject::getGlobalBounds() const
{
	if (this->isDestroyed())
	{
		static tps::IntRect emptyBounds{};
		return emptyBounds;
	}

	return this->m_GlobalBounds;
}

//----------------------------------------------------------------------------//
game::tps::IntRect game::StaticObject::getBoundsForBullet() const
{
	return this->getGlobalBounds();
}
