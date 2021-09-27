#pragma once

#include "system/types.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>
#include <string>

namespace game     {
namespace renderer {


class Texture2D : public sf::Texture
{
  public:
	typedef tps::IntRect SubTexture;

  public:
	Texture2D(const std::string& path);

	bool loaded() const;
	const SubTexture& getSubTexture(const std::string& name) const;
	void loadSubTextures(
		int subtexture_width,
		int subtexture_height,
		const std::vector<std::string>& subtextures_names
	);


  private:
	std::unordered_map<std::string, SubTexture>  m_subTextures;
	bool                                         m_isLoaded;

}; /* !class Texture2D */


} /* !namespace game */
} /* !namespace renderer */
