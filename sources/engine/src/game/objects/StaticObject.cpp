#include "StaticObject.hpp"


//----------------------------------------------------------------------------//
bool game::StaticObject::collides(const DynamicObject*) const
{
	return true;
}

//----------------------------------------------------------------------------//
game::tps::IntRect game::StaticObject::getBoundsForBullet() const
{
	return this->getGlobalBounds();
}
