#include "ResourcesManager.hpp"
#include "IEngine.hpp"
#include "system/log.hpp"
#include "system/utils.hpp"
#include "resources_path.h"
#include "renderer/ShaderProgram.hpp"
#include "renderer/Texture2D.hpp"
#include "renderer/Font.hpp"
#include "renderer/Sprite2D.hpp"

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <clocale>

namespace game {


ResourcesManager::ShadersMap             ResourcesManager::m_Shaders;
ResourcesManager::TexturesMap            ResourcesManager::m_Textures;
ResourcesManager::FontsMap               ResourcesManager::m_Fonts;
ResourcesManager::SpritesMap             ResourcesManager::m_Sprites;
ResourcesManager::TitlesDescriptionsMap  ResourcesManager::m_TitlesDescriptions;
std::vector<std::vector<std::string>>    ResourcesManager::m_LevelsDescriptions;
std::vector<std::array<unsigned, 4>>     ResourcesManager::m_EnemyTanksCounts;
std::vector<std::string>                 ResourcesManager::m_TitleDescription;
std::string                              ResourcesManager::m_ConfigPath;
std::string                              ResourcesManager::m_SavedConfig;

//----------------------------------------------------------------------------//
bool ResourcesManager::load(const std::string& relative_path)
{
	static bool isLoaded{ false };
	if (isLoaded)
	{ return true; }

	std::setlocale(LC_ALL, ".UTF-8");

	const std::string JSONString{ utils::fileToString(RESOURCE_PATH + relative_path) };
	if (JSONString.empty())
	{ return false;	}

	rapidjson::Document document{};
	rapidjson::ParseResult parseResult{ document.Parse(JSONString.c_str()) };
	if (!parseResult)
	{
		LOG_ERROR(
			"RESOURCES_MANAGER::JSON parse error: {0} ({1}), in file '{2}'",
			rapidjson::GetParseError_En(parseResult.Code()),
			parseResult.Offset(),
			relative_path
		);
		return false;
	}

	if (const auto itrConfigPath{ document.FindMember("gameConfigPath") };
		itrConfigPath != document.MemberEnd())
	{
		m_ConfigPath = itrConfigPath->value.GetString();
		m_SavedConfig = utils::fileToString(RESOURCE_PATH + m_ConfigPath);
	}

	if (const auto& itrShader{ document.FindMember("shaders") };
		itrShader != document.MemberEnd())
	{
		for (const auto& currentShader : itrShader->value.GetArray())
		{
			const std::string name{ currentShader["name"].GetString() };
			const std::string filePath_vs{ currentShader["filePath_vs"].GetString() };
			const std::string filePath_fs{ currentShader["filePath_fs"].GetString() };
			loadShader(name, filePath_vs, filePath_fs);
		}
	}

	if (const auto& itrTexture{ document.FindMember("textures") };
		itrTexture != document.MemberEnd())
	{
		for (const auto& currentTexture : itrTexture->value.GetArray())
		{
			const auto& subTexturesArray{ currentTexture["subTextures"].GetArray() };

			std::vector<std::string> subTexturesNames{};
			subTexturesNames.reserve(subTexturesArray.Size());

			for (const auto& currentSubTexture : subTexturesArray)
			{
				subTexturesNames.emplace_back(currentSubTexture.GetString());
			}

			loadTexture2D(
				currentTexture["name"].GetString(),
				currentTexture["filePath"].GetString(),
				currentTexture["subTextureWidth"].GetInt(),
				currentTexture["subTextureHeight"].GetInt(),
				subTexturesNames
			);
		}
	}

	if (const auto& itrFont{ document.FindMember("fonts") };
		itrFont != document.MemberEnd())
	{
		for (const auto& currentFont : itrFont->value.GetArray())
		{
			const auto& charactersArray{ currentFont["characters"].GetArray() };

			std::vector<std::string> charactersNames{};
			charactersNames.reserve(charactersArray.Size());

			for (const auto& currentSubTexture : charactersArray)
			{
				charactersNames.emplace_back(currentSubTexture.GetString());
			}

			loadFont(
				currentFont["name"].GetString(),
				currentFont["filePath"].GetString(),
				currentFont["characterSize"].GetInt(),
				charactersNames
			);
		}
	}

	if (const auto itrSprite{ document.FindMember("sprites") };
		itrSprite != document.MemberEnd())
	{
		for (const auto& currentSprite : itrSprite->value.GetArray())
		{
			std::vector<std::pair<std::string, unsigned>> tileDescriptions;
			
			if (const auto itrFrame{ currentSprite.FindMember("tiles") };
				itrFrame != currentSprite.MemberEnd())
			{ 
				const auto& tilesArray{ itrFrame->value.GetArray() };
				tileDescriptions.reserve(tilesArray.Size());
			
				for (const auto& currentFrame : tilesArray)
				{
					tileDescriptions.emplace_back(
						currentFrame["subTexture"].GetString(), currentFrame["duration"].GetUint()
					);
				}
			}
			else
			{
				tileDescriptions.emplace_back(currentSprite["name"].GetString(), 0);
			}

			loadSprite2D(
				currentSprite["name"].GetString(),
				currentSprite["shader"].GetString(),
				currentSprite["texture"].GetString(),
				tileDescriptions
			);
		}
	}

	if (const auto itrLevel{ document.FindMember("levels") };
		itrLevel != document.MemberEnd())
	{
		for (const auto& currentLevel : itrLevel->value.GetArray())
		{
			const auto& descriptionArray{ currentLevel["description"].GetArray() };
			std::vector<std::string> levelDescription{};
			levelDescription.reserve(descriptionArray.Size());

			for (const auto& currentRow : descriptionArray )
			{
				levelDescription.emplace_back(currentRow.GetString());
			}

			std::array<unsigned, 4> tanksCount{};
			unsigned index{};

			for (const auto& currentTankCount : currentLevel["enemy_tanks"].GetArray())
			{
				tanksCount[index] = currentTankCount.GetUint();
				
				if (++index == 4)
				{ break; }
			}

			loadLevel(levelDescription, tanksCount);
		}
	}

	if (const auto itrTitle{ document.FindMember("titles") };
		itrTitle != document.MemberEnd())
	{
		for (const auto& currentTitle : itrTitle->value.GetArray())
		{
			const auto& descriptionArray{ currentTitle["description"].GetArray() };
			std::vector<std::string> titleDescription{};
			titleDescription.reserve(descriptionArray.Size());

			for (const auto& currentRow : descriptionArray)
			{
				titleDescription.emplace_back(currentRow.GetString());
			}

			loadTitleDescription(currentTitle["name"].GetString(), titleDescription);
		}
	}
	
	isLoaded = true;

	return true;
}

//----------------------------------------------------------------------------//
void ResourcesManager::free()
{
	utils::stringToFile(RESOURCE_PATH + m_ConfigPath, m_SavedConfig);
	m_Shaders.clear();
	m_Textures.clear();
	m_Fonts.clear();
	m_Sprites.clear();
}

//----------------------------------------------------------------------------//
void ResourcesManager::saveConfig(const std::string& config)
{
	m_SavedConfig = config;
}

//----------------------------------------------------------------------------//
bool ResourcesManager::loadShader(
	const std::string& shader_name,
	const std::string& vertex_shader_path,
	const std::string& fragment_shader_path
)
{
	if (getShaderProgram(shader_name) != nullptr)
	{ return true; }

	const auto [itrShader, emplaced] {
		m_Shaders.try_emplace(
			shader_name,
			std::make_unique<renderer::ShaderProgram>(
				RESOURCE_PATH + vertex_shader_path, RESOURCE_PATH + fragment_shader_path
			)
		)
	};

	if (emplaced && itrShader->second->loaded())
	{ return true; }

	LOG_ERROR("RESOURCES_MANAGER::Can't load shader: '{}'", shader_name);
	return false;
}

//----------------------------------------------------------------------------//
renderer::ShaderProgram*
ResourcesManager::getShaderProgram(const std::string& shader_name)
{
	if (auto itrShader{ m_Shaders.find(shader_name) };
		itrShader != m_Shaders.end())
	{
		return itrShader->second.get();
	}

	return nullptr;
}

//----------------------------------------------------------------------------//
bool ResourcesManager::loadTexture2D(
	const std::string& texture_name,
	const std::string& relative_path,
	int subtexture_width,
	int subtexture_height,
	const std::vector<std::string>& subtextures_names
)
{
	m_Textures.try_emplace(
		texture_name,
		std::make_unique<renderer::Texture2D>(RESOURCE_PATH + relative_path)
	);

	if (auto texture2D{ getTexture2D(texture_name) };
		texture2D != nullptr && texture2D->loaded())
	{
		texture2D->loadSubTextures(
			subtexture_width, subtexture_height, subtextures_names
		);
		return true;
	}

	LOG_ERROR("RESOURCES_MANAGER::Can't load texture: '{}'", texture_name);
	return false;
}

//----------------------------------------------------------------------------//
bool ResourcesManager::loadFont(
	const std::string& font_name,
	const std::string& relative_path,
	int character_size,
	const std::vector<std::string>& characters_names
)
{
	m_Fonts.try_emplace(
		font_name,
		std::make_unique<renderer::Font>(RESOURCE_PATH + relative_path)
	);

	if (auto font{ getFont(font_name) };
		font != nullptr && font->loaded())
	{
		font->loadCharacters(character_size, characters_names);
		return true;
	}

	LOG_ERROR("RESOURCES_MANAGER::Can't load font: '{}'", font_name);
	return false;
}

//----------------------------------------------------------------------------//
renderer::Texture2D*
ResourcesManager::getTexture2D(const std::string& texture_name)
{
	if (auto itrTexture{ m_Textures.find(texture_name) };
		itrTexture != m_Textures.end())
	{
		return itrTexture->second.get();
	}

	return nullptr;
}

//----------------------------------------------------------------------------//
renderer::Font* ResourcesManager::getFont(const std::string& font_name)
{
	if (auto itrFont{ m_Fonts.find(font_name) };
		itrFont != m_Fonts.end())
	{
		return itrFont->second.get();
	}

	return nullptr;
}

//----------------------------------------------------------------------------//
bool ResourcesManager::loadSprite2D(
	const std::string& sprite_name,
	const std::string& shader_name,
	const std::string& texture_name,
	const std::vector<std::pair<std::string, unsigned>>& tiles_description
)
{
	auto texture{ getTexture2D(texture_name) };
	if (texture == nullptr)
	{ return false; }

	std::vector<renderer::Sprite2D::Tile> tiles;
	tiles.reserve(tiles_description.size());

	for (const auto& currentTileDescription : tiles_description)
	{
		tiles.emplace_back(
			texture->getSubTexture(currentTileDescription.first),
			currentTileDescription.second
		);
	}

	const auto [itrSprite, emplaced] {
		m_Sprites.try_emplace(
			sprite_name,
			std::make_unique<renderer::Sprite2D>(
				getShaderProgram(shader_name), getTexture2D(texture_name), std::move(tiles)
			)
		)
	};

	if (emplaced)
	{ return true; }

	LOG_ERROR("RESOURCES_MANAGER::Can't load sprite: '{}'", sprite_name);
	return false;
}

//----------------------------------------------------------------------------//
void ResourcesManager::loadLevel(
	std::vector<std::string>& level_rows, std::array<unsigned, 4> tanks_count
)
{
	m_LevelsDescriptions.emplace_back(std::move(level_rows));
	m_EnemyTanksCounts.emplace_back(tanks_count);
}

//----------------------------------------------------------------------------//
void ResourcesManager::loadTitleDescription(
	const std::string title_name,
	std::vector<std::string>& title_description
)
{
	std::size_t maxLength{};
	for (const auto& currentRow : title_description)
	{
		if (maxLength < title_description.back().length())
		{
			maxLength = title_description.back().length();
		}
	}
	for (auto& currentRow : title_description)
	{
		while (currentRow.length() < maxLength)
		{
			currentRow += '.';
		}
	}

	m_TitlesDescriptions.try_emplace(title_name, title_description);
}

//----------------------------------------------------------------------------//
renderer::Sprite2D* ResourcesManager::getSprite2D(const std::string& sprite_name)
{
	if (auto itrSprite{ m_Sprites.find(sprite_name) };
		itrSprite != m_Sprites.end())
	{
		return itrSprite->second.get();
	}

	LOG_ERROR("RESOURCES_MANAGER::Can't find sprite: '{}'", sprite_name);
	static renderer::Sprite2D emptySprite{};
	return std::addressof(emptySprite);
}

//----------------------------------------------------------------------------//
std::vector<std::string>& ResourcesManager::getLevelDescription(unsigned index)
{
	if (m_LevelsDescriptions.empty())
	{
		static std::vector<std::string> emptyLevel{};
		return emptyLevel;
	}

	if (index < m_LevelsDescriptions.size())
	{
		return m_LevelsDescriptions[index-1];
	}
	unsigned idx{ (index % m_LevelsDescriptions.size()) };
	return m_LevelsDescriptions[(index%m_LevelsDescriptions.size())];
}

//----------------------------------------------------------------------------//
const std::array<unsigned, 4>&
ResourcesManager::getEnemyTanksCount(unsigned index)
{
	if (m_EnemyTanksCounts.empty())
	{
		static std::array<unsigned, 4> countForEmptyLevel{ 20 };
		return countForEmptyLevel;
	}

	if (index < m_EnemyTanksCounts.size())
	{
		return m_EnemyTanksCounts[index-1];
	}
	return m_EnemyTanksCounts[(index%m_EnemyTanksCounts.size())];
}

//----------------------------------------------------------------------------//
std::vector<std::string>&
ResourcesManager::getTitleDescription(const std::string& title_name)
{
	if (auto itrTitle{ m_TitlesDescriptions.find(title_name) };
		itrTitle != m_TitlesDescriptions.end())
	{
		return itrTitle->second;
	}

	static std::vector<std::string> empty_description;
	return empty_description;
}

//----------------------------------------------------------------------------//
unsigned ResourcesManager::getSavedValue(const std::string& key)
{
	std::istringstream savedConfig{ m_SavedConfig };
	std::string        savedKey   {};
	unsigned           savedValue {};

	while (savedKey != key && savedConfig.good())
	{
		savedConfig >> savedKey;
	}

	savedConfig >> savedValue;
	return savedValue;
}


} /* !namespace game */


