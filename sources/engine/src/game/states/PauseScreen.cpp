#include "PauseScreen.hpp"
#include "IEngine.hpp"
#include "game/Core.hpp"
#include "game/InputHandler.hpp"
#include "system/log.hpp"
#include "game/objects/Title.hpp"
#include "game/ResourcesManager.hpp"
#include "renderer/Sprite2D.hpp"
#include "renderer/Text2D.hpp"

#include <algorithm>


//----------------------------------------------------------------------------//
game::PauseScreen::PauseScreen(Core* pCore)
    : Screen(pCore, EGameState::Pause)
{
    this->m_Title->setPosition(
        (WINDOW_WIDTH - this->m_Title->getSize().x) / 2, BLOCK_SIZE * 10
    );
    this->initText();
    this->m_ShowTimer.start(32);
}

//----------------------------------------------------------------------------//
void game::PauseScreen::updateInput()
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
                this->m_pCore->resume();
                isEnterPressed = true;
                return;
            case 1:
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
void game::PauseScreen::initText()
{
    this->m_MenuText->setString("CONTINUE\n\nEXIT");
    Screen::initText();
}

//----------------------------------------------------------------------------//
void game::PauseScreen::render() const
{
    Screen::render();
    if (this->m_isShowTime)
    {
        this->m_Title->render();
    }
}
