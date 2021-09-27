#include "Texture2D.hpp"
#include "system/log.hpp"

namespace game::renderer {

//----------------------------------------------------------------------------//
Texture2D::Texture2D(const std::string& path)
{
	this->m_isLoaded = sf::Texture::loadFromFile(path);
}

//----------------------------------------------------------------------------//
bool Texture2D::loaded() const
{
	return this->m_isLoaded;
}

//----------------------------------------------------------------------------//
const Texture2D::SubTexture&
Texture2D::getSubTexture(const std::string& name) const
{
	if (auto itrTexture{ this->m_subTextures.find(name) };
		itrTexture != this->m_subTextures.end())
	{
		return itrTexture->second;
	}

	LOG_WARN("Not found subtexture: '{}'", name);
	static const SubTexture defaultSubTexture{
		tps::Vec2i{},
		tps::Vec2i{
			static_cast<int>(sf::Texture::getSize().x),
			static_cast<int>(sf::Texture::getSize().y)
		}
	};

	return defaultSubTexture;
}

//----------------------------------------------------------------------------//
void Texture2D::loadSubTextures(
	int subtexture_width,
	int subtexture_height,
	const std::vector<std::string>& subtextures_names
)
{
	const int textureWidth{ static_cast<int>(sf::Texture::getSize().x) };

	int offsetX{};
	int offsetY{};

	for (const auto& name : subtextures_names)
	{
		this->m_subTextures.try_emplace(
			name, offsetX, offsetY, subtexture_width, subtexture_height
		);

		offsetX += subtexture_width;
		if (offsetX >= textureWidth)
		{
			offsetX = 0;
			offsetY += subtexture_height;
		}
	}
}


} /* !namespace game::renderer */