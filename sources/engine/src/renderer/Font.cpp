#include "Font.hpp"
#include "system/log.hpp"

namespace game::renderer {

//----------------------------------------------------------------------------//

Font::Font()
	: m_CharacterSize{ 0 }
	, m_isLoaded     { false }
{ }

//----------------------------------------------------------------------------//
Font::Font(const std::string& path)
	: m_CharacterSize{ 0 }
{
	this->m_isLoaded = sf::Texture::loadFromFile(path);
}

//----------------------------------------------------------------------------//
bool Font::loaded() const
{
	return this->m_isLoaded;
}

//----------------------------------------------------------------------------//
const Font::Character& Font::getCharacter(char symbol) const
{
	if (auto itrTexture{ this->m_Characters.find(symbol) };
		itrTexture != this->m_Characters.end())
	{
		return itrTexture->second;
	}

	LOG_WARN("Not found symbol: '{}'", symbol);
	static const Character emptyCharacter{};
	return emptyCharacter;
}

//----------------------------------------------------------------------------//
unsigned Font::getCharacterSize() const
{
	return this->m_CharacterSize;
}

//----------------------------------------------------------------------------//
void Font::loadCharacters(
	int character_size,
	const std::vector<std::string>& characters_names
)
{
	this->m_CharacterSize = character_size;
	const int textureWidth{ static_cast<int>(sf::Texture::getSize().x) };

	int offsetX{};
	int offsetY{};

	for (const auto& name : characters_names)
	{
		this->m_Characters.try_emplace(
			name.front(), offsetX, offsetY, character_size, character_size
		);

		offsetX += character_size;
		if (offsetX >= textureWidth)
		{
			offsetX = 0;
			offsetY += character_size;
		}
	}
}


} /* !namespace game::renderer */
