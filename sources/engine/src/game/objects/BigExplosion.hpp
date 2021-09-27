#pragma once

#include "StaticObject.hpp"
#include "renderer/Animator.hpp"


namespace game {

namespace renderer { class Sprite2D; }


class BigExplosion : public StaticObject
{
  public:
	BigExplosion(const tps::Vec2i& position, unsigned score);

	void render() const                  override;
	void update()                        override;


  private:
	renderer::Sprite2D*  m_pExplosionSprite;
	renderer::Sprite2D*  m_pScoreSprite;
	renderer::Animator   m_ExplosionAnimator;
	unsigned             m_ExplosionTimeLeft;
	unsigned             m_ScoreTimeLeft;

}; /* !class Border */


} /* !namespase game */
