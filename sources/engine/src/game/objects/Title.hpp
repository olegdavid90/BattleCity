#pragma once

#include "StaticObject.hpp"
#include "game/Core.hpp"
#include "system/types.hpp"

#include <vector>

namespace game {

namespace renderer { class Sprite2D; }


class Title : public StaticObject
{
  public:
	Title(EGameState game_state);
	
	void render() const;
	
	
  private:
	std::vector<std::pair<renderer::Sprite2D*, tps::Vec2i>>  m_Sprites;

}; /* !class Title */


} /* !namespace game */
