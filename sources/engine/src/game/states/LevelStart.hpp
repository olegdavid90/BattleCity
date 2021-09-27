#pragma once

#include "game/states/LevelState.hpp"

#include <memory>

namespace game {

namespace renderer {
	class Sprite2D;
	class Text2D;
} /* !namespace renderer */


class LevelStart : public LevelState
{
  public:
	LevelStart(class Level* parent_level);

	void render() const override;
	void update()       override;

  private:
	void initText()     override;


  private:
	std::unique_ptr<renderer::Text2D>  m_pStageText;

}; /* !class LevelStart */


} /* !namespase game */
