#pragma once

#include "IEngine.hpp"
#include "system/types.hpp"
#include "system/NonCopyable.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <cstdint>
#include <vector>
#include <string>
#include <utility>

namespace game     {
namespace renderer {


class Text2D : public sf::RectangleShape
{
  public:
	Text2D(const std::string& font_name);

	void setString(const std::string& string);
	void setPosition(tps::Vec2f position);
	void setPosition(float x, float y);
	void setCharacterSize(unsigned character_size);
	void render();
	void setFillColor(
		std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha = 255
	);
	tps::Vec2f getSize()            const;
	const tps::Vec2f& getPosition() const;

  private:
	const class Font*     m_pFont;
	class ShaderProgram*  m_pShader;
	std::string           m_String;
	tps::Vec4f            m_FillColor;											  
	tps::Vec2f            m_Position;
	float                 m_CharacterSize;	
	unsigned              m_ColumnCount;
	unsigned              m_RowCount;
  
}; /* !class Text */

	
} /* !namespace renderer */
} /* !namespace game */
