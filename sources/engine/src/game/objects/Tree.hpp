#pragma once

#include "StaticObject.hpp"

namespace game {
	
namespace renderer { class Sprite2D; }


class Tree : public StaticObject
{
  public:
	Tree(const tps::Vec2i& position);

	void render() const override;


  private:
	renderer::Sprite2D*  m_pSprite;
	
}; /* !class Tree */


} /* !namespace game */
