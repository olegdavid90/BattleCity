#include "StartScreen.hpp"
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
game::StartScreen::StartScreen(Core* pCore)
    : Screen(pCore, EGameState::StartScreen)
    , m_isFirstLevel{ !static_cast<bool>(ResourcesManager::getSavedValue("last_level")) }
{
    this->m_Title->setPosition(
        (WINDOW_WIDTH - this->m_Title->getSize().x) / 2, BLOCK_SIZE * 7
    );
    if (this->m_isFirstLevel == false)
    {
        this->m_TankPosition.y = BLOCK_SIZE * 18;
        this->m_maxMenuSelection = 2;
        this->m_MenuSelection = 1;
    }
    this->initText();
}

//----------------------------------------------------------------------------//
void game::StartScreen::updateInput()
{
    Screen::updateInput();

    static bool isEnterPressed{ this->m_Keys[KEY_ENTER] };
    if (this->m_Keys[KEY_ENTER])
    {
        if (isEnterPressed == false)
        {
            switch (this->m_MenuSelection)
            {
            case 0:
                this->m_pCore->startNewGame();
                isEnterPressed = true;
                break;
            case 1:
                if (this->m_isFirstLevel)
                {
                    this->m_pCore->exit();
                }
                else
                {
                    this->m_pCore->loadConfig();
                    this->m_pCore->startNextLevel();
                }
                break;
            case 2:
                this->m_pCore->exit();
            }
        }
    }
    else
    {
        isEnterPressed = false;
    }
}

//----------------------------------------------------------------------------//
void game::StartScreen::initText()
{
    std::ostringstream osstrm{};
    const unsigned playerScore{ ResourcesManager::getSavedValue("player_score") };
    unsigned highScore        { ResourcesManager::getSavedValue("high_score") };

    osstrm << "PLAYER—" << std::setw(6) << (playerScore ? std::to_string(playerScore) : "00")
           << "  HI—"   << std::setw(6) << (highScore   ? std::to_string(highScore)   : "00");

    this->m_ScoreText->setFillColor(255, 255, 255);
    this->m_ScoreText->setString(osstrm.str());
    this->m_ScoreText->setPosition(
        (WINDOW_WIDTH - this->m_ScoreText->getSize().x) / 2,
        BLOCK_SIZE * 3
    );

    if (this->m_isFirstLevel)
    {
        this->m_MenuText->setString("NEW GAME\n\nEXIT");
    }
    else
    {
        this->m_MenuText->setString("NEW GAME\n\nCONTINUE\n\nEXIT");
    }
    Screen::initText();
}

//----------------------------------------------------------------------------//
void game::StartScreen::render() const
{
    Screen::render();
    this->m_Title->render();
    this->m_ScoreText->render();
}
