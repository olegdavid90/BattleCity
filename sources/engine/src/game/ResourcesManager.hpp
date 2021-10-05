#pragma once

#include <unordered_map>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <array>

namespace game {

namespace renderer {
	class ShaderProgram;
	class Texture2D;
	class Font;
	class Sprite2D;
} /* !namespace renderer */

namespace audio { class SoundBuffer; }

class ResourcesManager
{
  private:
	typedef std::unordered_map<std::string, std::unique_ptr<renderer::ShaderProgram>>
		ShadersMap;
	typedef std::unordered_map<std::string, std::unique_ptr<renderer::Texture2D>>
		TexturesMap;
	typedef std::unordered_map<std::string, std::unique_ptr<renderer::Font>>
		FontsMap;
	typedef std::unordered_map<std::string, std::unique_ptr<renderer::Sprite2D>> 
		SpritesMap;
	typedef std::unordered_map<std::string, std::unique_ptr<audio::SoundBuffer>>
		SoundBuffersMap;
	typedef std::unordered_map<std::string, std::vector<std::string>>
		TitlesDescriptionsMap;

  public:
	ResourcesManager() = delete;

	static bool load(const std::string& JSON_path);
	static void free();
	static void saveConfig(const std::string& config);
	static renderer::ShaderProgram* getShaderProgram(const std::string& shader_name);
	static renderer::Texture2D* getTexture2D(const std::string& texture_name);
	static renderer::Font* getFont(const std::string& font_name);
	static renderer::Sprite2D* getSprite2D(const std::string& sprite_name);
	static std::vector<std::string>& getLevelDescription(unsigned index);
	static std::vector<std::string>& getTitleDescription(const std::string& title_name);
	static const std::array<unsigned, 4>& getEnemyTanksCount(unsigned index);
	static unsigned getSavedValue(const std::string& key);

  private:
	static bool loadShader(
		const std::string& shader_name,
		const std::string& vertex_shader_path,
		const std::string& fragment_shader_path
	);
	static bool loadTexture2D(
		const std::string& texture_name,
		const std::string& relative_path,
		int subtexture_width,
		int subtexture_height,
		const std::vector<std::string>& subtextures_names
	);
	static bool loadFont(
		const std::string& font_name,
		const std::string& relative_path,
		int character_size,
		const std::vector<std::string>& characters_names
	);
	static bool loadSprite2D(
		const std::string& sprite_name,
		const std::string& shader_name,
		const std::string& texture_name,
		const std::vector<std::pair<std::string, unsigned>>& tiles_description
	);
	static void loadLevel(
		std::vector<std::string>& level_rows,
		std::array<unsigned, 4> tanks_count
	);
	static void loadTitleDescription(
		const std::string title_name,
		std::vector<std::string>& title_description
	);


  private:
	static ShadersMap                             m_Shaders;
	static TexturesMap                            m_Textures;
	static FontsMap                               m_Fonts;
	static SpritesMap                             m_Sprites;
	static SoundBuffersMap                        m_SoundBuffers;
	static TitlesDescriptionsMap                  m_TitlesDescriptions;
	static std::vector<std::vector<std::string>>  m_LevelsDescriptions;
	static std::vector<std::array<unsigned, 4>>   m_EnemyTanksCounts;
	static std::vector<std::string>               m_TitleDescription;
	static std::string                            m_ConfigPath;
	static std::string                            m_SavedConfig;

}; /* !class ResourcesManager */


} /* !namespace game */
