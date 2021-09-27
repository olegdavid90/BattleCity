#include "ShaderProgram.hpp"
#include "system/log.hpp"

namespace game::renderer {


ShaderProgram::ShaderProgram(
	const std::string& vertex_shader_path,
	const std::string& fragment_shader_path
)
{
	this->m_isLoaded =
		sf::Shader::isAvailable() 
		//&& sf::Shader::loadFromFile(vertex_shader_path, sf::Shader::Vertex)
		&& sf::Shader::loadFromFile(fragment_shader_path, sf::Shader::Fragment);
}


bool ShaderProgram::loaded() const
{
	return this->m_isLoaded;
}


} /* !namespace game::renderer */
