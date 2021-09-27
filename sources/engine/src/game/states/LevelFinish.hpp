#pragma once

#include "game/states/LevelState.hpp"

#include <memory>
#include <array>

namespace game {

namespace renderer {
	class Sprite2D;
	class Text2D;
} /* !namespace renderer */


class LevelFinish : public LevelState
{
  public:
	LevelFinish(class Level* parent_level);

	void render() const override;
	void update()       override;

  private:
	void initText()     override;


private:
	tps::Vec2i                                        m_TankPosition;
	std::array<renderer::Sprite2D*, 4>                m_pTanksSprites;
	renderer::Sprite2D*                               m_pBlackFill;
	std::array<std::unique_ptr<renderer::Text2D>, 6>  m_pTexts;

}; /* !class LevelFinish */


} /* !namespase game */
