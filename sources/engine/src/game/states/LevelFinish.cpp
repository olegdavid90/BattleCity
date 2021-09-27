#include "LevelFinish.hpp"
#include "system/log.hpp"
#include "renderer/Sprite2D.hpp"
#include "renderer/Text2D.hpp"
#include "game/states/Level.hpp"
#include "game/ResourcesManager.hpp"

#include <string>
#include <sstream>
#include <iomanip>


//----------------------------------------------------------------------------//
game::LevelFinish::LevelFinish(class Level* parent_level)
	: LevelState(parent_level, tps::Vec2i{ WINDOW_WIDTH, 0 })
	, m_TankPosition { BLOCK_SIZE * 22, BLOCK_SIZE * 11 }
	, m_pTanksSprites{
		ResourcesManager::getSprite2D("enemy_000"),
		ResourcesManager::getSprite2D("enemy_100"),
		ResourcesManager::getSprite2D("enemy_200"),
		ResourcesManager::getSprite2D("enemy_300")
	}
	, m_pBlackFill   { ResourcesManager::getSprite2D("black_fill") }
{
	for (auto& currentText : this->m_pTexts)
	{
		currentText = std::make_unique<renderer::Text2D>("pixel_font");
	}
	this->initText();
	this->m_pBorder->setSize(this->m_BorderSize);
	this->m_pBlackFill->setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	for (auto currentTankSprite : this->m_pTanksSprites)
	{
		currentTankSprite->setSize(BLOCK_SIZE * 2, BLOCK_SIZE * 2);
	}
	this->m_StateTimer.setCallback(
		[this]()
		{
			if (this->m_pParentLevel->isGameOver())
			{
				this->m_pParentLevel->finishGameOver();
			}
			else
			{
				this->m_eStateType = ELevelStateType::ShowBorder;
			}
		}
	);
	this->m_StateTimer.start(5);
}

//----------------------------------------------------------------------------//
void game::LevelFinish::render() const
{
	if (this->m_eStateType == ELevelStateType::ShowInfo)
	{
		this->m_pBlackFill->render(tps::Vec2i{});
		
		for (auto& currentText : this->m_pTexts)
		{
			currentText->render();
		}
		for (int index{}; index < this->m_pTanksSprites.size(); ++index)
		{
			this->m_pTanksSprites[index]->render(
				tps::Vec2i{
					this->m_TankPosition.x,
					this->m_TankPosition.y + BLOCK_SIZE * index * 3
				}
			);
		}
	}
	else
	{
		this->m_pBorder->render(tps::Vec2i{}, this->m_BorderSize);
		this->m_pBorder->render(
			tps::Vec2i{ 0, WINDOW_HEIGHT - this->m_BorderSize.y },
			this->m_BorderSize
		);
	}
}

//----------------------------------------------------------------------------//
void game::LevelFinish::update()
{
	this->m_StateTimer.update();

	if (this->m_eStateType == ELevelStateType::ShowBorder)
	{
		this->m_BorderSize.y += BLOCK_SIZE;
		if (this->m_BorderSize.y > WINDOW_HEIGHT / 2)
		{
			this->m_pParentLevel->nextLevel();
		}
	}
}

//----------------------------------------------------------------------------//
void game::LevelFinish::initText()
{
	std::ostringstream osstrm{};

	this->m_pTexts[0]->setString("HI—SCORE");
	this->m_pTexts[0]->setFillColor(216, 40, 0);
	this->m_pTexts[0]->setPosition(
		WINDOW_WIDTH / 2 - this->m_pTexts[0]->getSize().x,
		BLOCK_SIZE * 2
	);

	osstrm << std::setw(8) << ResourcesManager::getSavedValue("high_score");
	this->m_pTexts[1]->setString(osstrm.str());
	this->m_pTexts[1]->setFillColor(252, 228, 160);
	this->m_pTexts[1]->setPosition(
		WINDOW_WIDTH / 2,
		BLOCK_SIZE * 2
	);

	osstrm.str("");
	osstrm << "STAGE " << std::setw(2) << this->m_pParentLevel->getIndex();
	this->m_pTexts[2]->setString(osstrm.str());
	this->m_pTexts[2]->setFillColor(255, 255, 255);
	this->m_pTexts[2]->setPosition(
		(WINDOW_WIDTH - this->m_pTexts[2]->getSize().x) / 2,
		BLOCK_SIZE * 4
	);

	this->m_pTexts[3]->setString("PLAYER  ");
	this->m_pTexts[3]->setFillColor(216, 40, 0);
	this->m_pTexts[3]->setPosition(
		WINDOW_WIDTH / 2 - m_pTexts[3]->getSize().x,
		BLOCK_SIZE * 7
	);

	osstrm.str("");
	osstrm << std::setw(8) << this->m_pParentLevel->getPlayerScore();
	this->m_pTexts[4]->setString(osstrm.str());
	this->m_pTexts[4]->setFillColor(252, 228, 160);
	this->m_pTexts[4]->setPosition(
		WINDOW_WIDTH / 2,
		BLOCK_SIZE * 7
	);

	const auto& destroyedEnemiesCount{ this->m_pParentLevel->getDestroyedEnemiesCounts() };
	osstrm.str("");
	osstrm << std::setw(4)  << destroyedEnemiesCount[0] * 100 << " PTS"
		   << std::setw(5)  << destroyedEnemiesCount[0] << "l\n\n\n"
		   << std::setw(4)  << destroyedEnemiesCount[1] * 200 << " PTS"
		   << std::setw(5)  << destroyedEnemiesCount[1] << "l\n\n\n"
		   << std::setw(4)  << destroyedEnemiesCount[2] * 300 << " PTS"
		   << std::setw(5)  << destroyedEnemiesCount[2] << "l\n\n\n"
		   << std::setw(4)  << destroyedEnemiesCount[3] * 400 << " PTS"
		   << std::setw(5)  << destroyedEnemiesCount[3] << "l\n"
		   << std::setw(14) << "____" << "\n   TOTAL"
		   << std::setw(5)  << destroyedEnemiesCount[4];
	this->m_pTexts[5]->setString(osstrm.str());
	this->m_pTexts[5]->setFillColor(255, 255, 255);
	this->m_pTexts[5]->setPosition(
		this->m_TankPosition.x - m_pTexts[5]->getSize().x,
		this->m_TankPosition.y + BLOCK_SIZE / 2.F
	);
}
