#pragma once

#include "StaticObject.hpp"

namespace game {
	
namespace renderer { class Sprite2D; }


class Ice : public StaticObject
{
  public:
	Ice(const tps::Vec2i& position);

	void render()                              const override;
	bool collides(const DynamicObject* object) const override;


  private:
	renderer::Sprite2D*        m_pSprite;

}; /* !class Ice */


} /* !namespace game */
