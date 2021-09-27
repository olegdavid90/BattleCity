#pragma once

#include "StaticObject.hpp"
#include "renderer/Animator.hpp"

namespace game {

namespace renderer { class Sprite2D; }


class Water : public StaticObject
{
  public:
	Water(const tps::Vec2i& position);

	void update()                                    override;
	void render()                              const override;
	bool collides(const DynamicObject* object) const override;


  private:
	class renderer::Sprite2D*  m_pSprite;
	renderer::Animator         m_Animator;

}; /* !class Water */


} /* !namespace game */
