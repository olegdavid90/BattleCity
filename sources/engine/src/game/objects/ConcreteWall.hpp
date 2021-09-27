#pragma once

#include "StaticObject.hpp"

namespace game {

namespace renderer { class Sprite2D; }


class ConcreteWall : public StaticObject
{
  public:
	ConcreteWall(const tps::Vec2i& position);

	void onCollision(DynamicObject* object) override;
	void render() const override;


  private:
	renderer::Sprite2D*  m_pSprite;

}; /* !class ConcreteWall */


} /* !namespace game */
