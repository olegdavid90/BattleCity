#pragma once

#include <SFML/Graphics/Shader.hpp>
#include <string>

namespace game     {
namespace renderer {


class ShaderProgram : public sf::Shader
{
  public:
	ShaderProgram(
		const std::string& vertex_shader_path,
		const std::string& fragment_shader_path
	);

	bool loaded() const;


  private:
	bool  m_isLoaded;

}; /* !class ShaderProgram */


} /* !namespace renderer */
} /* !namespace game */
