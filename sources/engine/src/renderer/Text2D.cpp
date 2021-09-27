#include "Text2D.hpp"
#include "IEngine.hpp"
#include "Font.hpp"
#include "MainWindow.hpp"
#include "system/log.hpp"
#include "ShaderProgram.hpp"
#include "game/ResourcesManager.hpp"

namespace game::renderer {

//----------------------------------------------------------------------------//
Text2D::Text2D(const std::string& font_name)
	: m_pFont        { ResourcesManager::getFont(font_name) }
	, m_pShader      { ResourcesManager::getShaderProgram("textShader") }
	, m_FillColor    { 0.F, 0.F, 0.F, 1.F }
	, m_CharacterSize{ static_cast<float>(m_pFont->getCharacterSize()) }
	, m_ColumnCount  {}
	, m_RowCount     {}
{
	sf::RectangleShape::setTexture(this->m_pFont);
	sf::RectangleShape::setSize(
		tps::Vec2f{this->m_CharacterSize, this->m_CharacterSize }
	);
}

//----------------------------------------------------------------------------//
void Text2D::setString(const std::string& string)
{
	this->m_String.clear();
	this->m_String = string;

	if (this->m_String.empty())
	{ return; }

	unsigned currentColumnCount{};
	this->m_RowCount = 1;

	for (auto character : this->m_String)
	{
		if (character == '\n')
		{
			currentColumnCount = 0;
			++this->m_RowCount;
			continue;
		}

		this->m_ColumnCount =
			(this->m_ColumnCount > ++currentColumnCount) ? this->m_ColumnCount :
			                                               currentColumnCount  ;
	}
}

//----------------------------------------------------------------------------//
void Text2D::setPosition(tps::Vec2f position)
{
	this->m_Position = position;
}

//----------------------------------------------------------------------------//
void Text2D::setPosition(float x, float y)
{
	this->setPosition(tps::Vec2f{ x, y });
}

//----------------------------------------------------------------------------//
void Text2D::setCharacterSize(unsigned character_size)
{
	this->m_CharacterSize = static_cast<float>(character_size);
	sf::RectangleShape::setSize(
		tps::Vec2f{ this->m_CharacterSize, this->m_CharacterSize }
	);
}

//----------------------------------------------------------------------------//
void Text2D::setFillColor(
	std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha
)
{
	this->m_FillColor = { red / 255.F, green / 255.F, blue / 255.F, alpha / 255.F };
}

//----------------------------------------------------------------------------//
void Text2D::render()
{
	tps::Vec2f currentPosition{ this->m_Position };

	for (auto symbol : this->m_String)
	{
		if (symbol == '\n')
		{
			currentPosition.y += this->m_CharacterSize;
			currentPosition.x  = this->m_Position.x;
			continue;
		}

		sf::RectangleShape::setPosition(currentPosition);
		currentPosition.x += this->m_CharacterSize;

		this->m_pShader->setUniform("fillColor", this->m_FillColor);
		sf::RectangleShape::setTextureRect(this->m_pFont->getCharacter(symbol));
		MainWindow::instance()->draw(*this, this->m_pShader);
	}
}

//----------------------------------------------------------------------------//
tps::Vec2f Text2D::getSize() const
{
	return tps::Vec2f{
		this->m_ColumnCount * this->m_CharacterSize,
		this->m_RowCount    * this->m_CharacterSize,
	};
}

//----------------------------------------------------------------------------//
const tps::Vec2f& Text2D::getPosition() const
{
	return this->m_Position;
}


} /* !namespace game::renderer */
