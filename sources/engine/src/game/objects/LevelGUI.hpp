#pragma once

#include "system/types.hpp"

#include <memory>
#include <vector>

namespace game {

namespace renderer {
	class Sprite2D;
	class Text2D;
} /* !namespace renderer */


class LevelGUI
{
  public:
	LevelGUI(const class Level* parent_level);
	
	void render(unsigned passive_tanks_count) const;
    void update();
	
  private:
	const class Level*                       m_pParentLevel;
	std::vector<tps::Vec2i>                  m_EnemyIconPositions;
	tps::Vec2i                               m_PlayerIconPosition;
	tps::Vec2i                               m_LevelIconPosition;

	std::unique_ptr<class renderer::Text2D>  m_PlayerHealthText;
	std::unique_ptr<class renderer::Text2D>  m_LevelIndexText;

	renderer::Sprite2D*                      m_pEnemyIconSprite;
	renderer::Sprite2D*                      m_pPlayerIconSprite;
	renderer::Sprite2D*                      m_pLevelIconSprite;
  
}; /* class LevelGUI */


} /* !namespace game */
