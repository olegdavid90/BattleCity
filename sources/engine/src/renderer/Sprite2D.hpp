#pragma once

#include "system/types.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>

namespace game     {
namespace renderer {


class Sprite2D : public sf::RectangleShape
{
  public:
	struct Tile
	{
		Tile(const tps::IntRect& int_rect, unsigned duration)
			: TextureRect{ int_rect }
			, duration   { duration }
		{ }

		tps::IntRect TextureRect;
		unsigned     duration;
	};

  public:
	Sprite2D();
	Sprite2D(
		class ShaderProgram* shader,
		const class Texture2D* pTexture2D,
		std::vector<Tile> frames
	);

	void render(const tps::Vec2i& position, const tps::Vec2i& size);
	void render(const tps::Vec2i& position, unsigned tile_number);
	void render(const tps::Vec2i& position);
	void setSize(const tps::Vec2i& size);
	void setSize(int width, int height);
	unsigned getTileDuration(unsigned frame_id) const;
	unsigned getTilesCount() const;

  private:
	void render();

  private:
	class ShaderProgram*  m_pShader;
	std::vector<Tile>     m_Tiles;

}; /* !class Sprite2D */


} /* !namespace game */
} /* !namespace render_engine */
