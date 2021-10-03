#pragma once

#include "system/types.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>
#include <string>

namespace game     {
namespace renderer {


class Font : public sf::Texture
{
  public:
	typedef tps::IntRect Character;

  public:
	Font();
	Font(const std::string& path);

	bool loaded()                              const;
	const Character& getCharacter(char symbol) const;
	unsigned getCharacterSize()                const;
	void loadCharacters(
		int character_size,
		const std::vector<std::string>& characters_names
	);


  private:
	std::unordered_map<char, Character>  m_Characters;
	unsigned                             m_CharacterSize;
	bool                                 m_isLoaded;

	
}; /* !class Font */


} /* !namespace renderer */
} /* !namespace game */
