#include "LevelGUI.hpp"
#include "IEngine.hpp"
#include "game/states/Level.hpp"
#include "system/log.hpp"
#include "renderer/Text2D.hpp"
#include "renderer/Sprite2D.hpp"
#include "game/ResourcesManager.hpp"

#include <string>


//----------------------------------------------------------------------------//
game::LevelGUI::LevelGUI(const Level* parent_level)
	: m_pParentLevel        { parent_level }
	, m_PlayerIconPosition  { WINDOW_WIDTH - BLOCK_SIZE * 3, BLOCK_SIZE * 15 }
	, m_LevelIconPosition   { WINDOW_WIDTH - BLOCK_SIZE * 3, BLOCK_SIZE * 22 }
	, m_PlayerHealthText    { std::make_unique<renderer::Text2D>("pixel_font") }
	, m_LevelIndexText      { std::make_unique<renderer::Text2D>("pixel_font") }
	, m_pEnemyIconSprite    { ResourcesManager::getSprite2D("enemy_tank_icon") }
	, m_pPlayerIconSprite   { ResourcesManager::getSprite2D("player_tank_icon") }
	, m_pLevelIconSprite    { ResourcesManager::getSprite2D("level_icon") }
{
	this->m_EnemyIconPositions.resize(20);
	const int leftOffset{ WINDOW_WIDTH - BLOCK_SIZE * 3 };
	const int topOffset { BLOCK_SIZE * 2 };

	this->m_pEnemyIconSprite ->setSize(BLOCK_SIZE, BLOCK_SIZE);
	this->m_pPlayerIconSprite->setSize(BLOCK_SIZE, BLOCK_SIZE);
	this->m_pLevelIconSprite ->setSize(BLOCK_SIZE, BLOCK_SIZE);

	for (int index{}; index < this->m_EnemyIconPositions.size(); ++index)
	{
		const int positionX{ leftOffset + BLOCK_SIZE * (index % 2) };
		const int positionY{ topOffset  + BLOCK_SIZE * (index / 2) };

		this->m_EnemyIconPositions[index] = { positionX, positionY };
	}
}

//----------------------------------------------------------------------------//
void game::LevelGUI::update()
{
	const int      playerHealth{ this->m_pParentLevel->getPlayerHealth() };
    const unsigned levelIndex  { this->m_pParentLevel->getIndex() };

	this->m_LevelIndexText->setString(std::to_string(levelIndex));
	this->m_PlayerHealthText->setString(
		(playerHealth < 0) ? "" : std::to_string(playerHealth)
	);

	this->m_PlayerHealthText->setPosition(
		WINDOW_WIDTH - BLOCK_SIZE - this->m_PlayerHealthText->getSize().x,
		BLOCK_SIZE * 15
	);
	this->m_LevelIndexText->setPosition(
		WINDOW_WIDTH - BLOCK_SIZE - this->m_LevelIndexText->getSize().x,
		BLOCK_SIZE * 24
	);
}

//----------------------------------------------------------------------------//
void game::LevelGUI::render(unsigned passive_tanks_count) const
{
	for (unsigned index{}; index < passive_tanks_count; ++index)
	{
		m_pEnemyIconSprite->render(this->m_EnemyIconPositions[index]);
	}

	if (m_PlayerHealthText->getSize().x <= BLOCK_SIZE)
	{
		this->m_pPlayerIconSprite->render(this->m_PlayerIconPosition);
	}

	this->m_pLevelIconSprite->render(this->m_LevelIconPosition);
	this->m_PlayerHealthText->render();
	this->m_LevelIndexText->render();
}
