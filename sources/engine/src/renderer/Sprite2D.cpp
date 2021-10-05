#include "Sprite2D.hpp"
#include "IEngine.hpp"
#include "system/log.hpp"
#include "Texture2D.hpp"
#include "renderer/MainWindow.hpp"
#include "renderer/ShaderProgram.hpp"

namespace game::renderer {

//----------------------------------------------------------------------------//
Sprite2D::Sprite2D()
	: m_pShader{ nullptr }
{ }

//----------------------------------------------------------------------------//
Sprite2D::Sprite2D(
	ShaderProgram* shader,
	const Texture2D* pTexture2D,
	std::vector<Tile> frames
)
	: m_pShader{ shader }
	, m_Tiles  { std::move(frames) }
{
	sf::RectangleShape::setTexture(pTexture2D);
	sf::RectangleShape::setTextureRect(this->m_Tiles.front().TextureRect);
}

//----------------------------------------------------------------------------//
void Sprite2D::render(const tps::Vec2i& position, const tps::Vec2i& size)
{
	this->setSize(size);
	this->render(position);
}

//----------------------------------------------------------------------------//
void Sprite2D::render(const tps::Vec2i& position, unsigned tile_number)
{
	if (tile_number < this->m_Tiles.size())
	{
		sf::RectangleShape::setTextureRect(this->m_Tiles[tile_number].TextureRect);
	}
	this->render(position);
}

//----------------------------------------------------------------------------//
void Sprite2D::render(const tps::Vec2i& position)
{
	sf::RectangleShape::setPosition(static_cast<tps::Vec2f>(position));
	this->render();
}

//----------------------------------------------------------------------------//
void Sprite2D::render()
{
	MainWindow::instance()->draw(*this, this->m_pShader);
}

//----------------------------------------------------------------------------//
unsigned Sprite2D::getTileDuration(unsigned frame_id) const
{
	return (frame_id < this->m_Tiles.size()) ? this->m_Tiles[frame_id].duration : 0;
}

//----------------------------------------------------------------------------//
unsigned Sprite2D::getTilesCount() const
{
	return static_cast<unsigned>(this->m_Tiles.size());
}

//----------------------------------------------------------------------------//
void Sprite2D::setSize(const tps::Vec2i& size)
{
	sf::RectangleShape::setSize(static_cast<tps::Vec2f>(size));
}

//----------------------------------------------------------------------------//
void Sprite2D::setSize(int width, int height)
{
	this->setSize(tps::Vec2i{ width, height });
}


} /* !namespace game::renderer */
