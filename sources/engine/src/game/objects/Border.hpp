#pragma once

#include "StaticObject.hpp"

namespace game {

namespace renderer { class Sprite2D; }


class Border : public StaticObject
{
  public:
	Border(const tps::Vec2i& position, const tps::Vec2i& size);

	void render() const override;


  private:
	renderer::Sprite2D*  m_pSprite;

}; /* !class Border */


} /* !namespase game */
