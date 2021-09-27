#include "GameOverScreen.hpp"
#include "IEngine.hpp"
#include "game/Core.hpp"
#include "game/InputHandler.hpp"
#include "system/log.hpp"
#include "game/objects/Title.hpp"
#include "game/ResourcesManager.hpp"
#include "renderer/Sprite2D.hpp"
#include "renderer/Text2D.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>


//----------------------------------------------------------------------------//
game::GameOverScreen::GameOverScreen(Core* pCore)
    : Screen(pCore, EGameState::GameOver)
{
    this->m_TankPosition += tps::Vec2i{ BLOCK_SIZE * 2, BLOCK_SIZE * 2 };
    this->m_isShowTime       = false;
    this->m_maxMenuSelection = 0;
    this->m_Title->setPosition(
        (WINDOW_WIDTH - this->m_Title->getSize().x) / 2, BLOCK_SIZE * 8
    );
    this->initText();
}

//----------------------------------------------------------------------------//
void game::GameOverScreen::updateInput()
{
    Screen::updateInput();

    static bool isEnterPressed{ this->m_Keys[KEY_ENTER] };
    if (this->m_Keys[KEY_ENTER])
    {
        if (isEnterPressed == false)
        {
            this->m_pCore->exit();
        }
    }
    else
    {
        isEnterPressed = false;
    }
}

//----------------------------------------------------------------------------//
void game::GameOverScreen::render() const
{
    Screen::render();
    this->m_Title->render();

    if (this->m_isShowTime)
    {
        this->m_ScoreText->render();
    }
}

//----------------------------------------------------------------------------//
void game::GameOverScreen::initText()
{
    const unsigned playerScore{ ResourcesManager::getSavedValue("player_score") };
    const unsigned highScore  { ResourcesManager::getSavedValue("high_score") };

    if (playerScore > highScore)
    {
        this->m_pCore->saveNewHighScore(playerScore);
        this->m_ScoreText->setCharacterSize(BLOCK_SIZE * 2);
        this->m_ScoreText->setFillColor(216, 40, 0);
        this->m_ScoreText->setString("HISCORE " + std::to_string(playerScore));
        this->m_ScoreText->setPosition(
            (WINDOW_WIDTH - this->m_ScoreText->getSize().x) / 2,
            BLOCK_SIZE * 3
        );
        this->m_isShowTime = true;
        this->m_ShowTimer.start(32);
    }

    this->m_MenuText->setString("EXIT");
    Screen::initText();
}
