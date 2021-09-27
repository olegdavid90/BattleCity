#include "Screen.hpp"
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
game::Screen::Screen(Core* pCore, EGameState screen_type)
    : GameState(pCore)
    , m_TankPosition    { BLOCK_SIZE * 9, BLOCK_SIZE * 19 }
    , m_Title           { std::make_unique<Title>(screen_type) }
    , m_MenuText        { std::make_unique<renderer::Text2D>("pixel_font") }
    , m_ScoreText       { std::make_unique<renderer::Text2D>("pixel_font") }
    , m_pTankSprite     { ResourcesManager::getSprite2D("player_03") }
    , m_TankAnimator    { this->m_pTankSprite }
    , m_maxMenuSelection{ 1 }
    , m_MenuSelection   {}
    , m_Keys            { false }
    , m_isShowTime      { true }
{
    this->m_pTankSprite->setSize(BLOCK_SIZE * 2, BLOCK_SIZE * 2);
    this->m_ShowTimer.setCallback(
        [this]
        {
            this->m_isShowTime = !this->m_isShowTime;
            if (this->m_isShowTime)
            {
                this->m_ShowTimer.start(32);
            }
            else
            {
                this->m_ShowTimer.start(16);
            }
        }
    );
}

//----------------------------------------------------------------------------//
game::Screen::~Screen()
{ }

//----------------------------------------------------------------------------//
void game::Screen::update()
{
    this->m_TankAnimator.update();
    this->m_ShowTimer.update();
}

//----------------------------------------------------------------------------//
void game::Screen::render() const
{
    const tps::Vec2i currentTankPosition{
        this->m_TankPosition.x,
        this->m_TankPosition.y + this->m_MenuSelection * BLOCK_SIZE * 2
    };
    this->m_pTankSprite->render(
        currentTankPosition, this->m_TankAnimator.getCurrentTile()
    );
    this->m_MenuText->render();
}

//----------------------------------------------------------------------------//
void game::Screen::initText()
{
    this->m_MenuText->setFillColor(255, 255, 255);
    this->m_MenuText->setPosition(
        this->m_TankPosition.x + BLOCK_SIZE * 4.F,
        this->m_TankPosition.y + BLOCK_SIZE / 2.F
    );
}

//----------------------------------------------------------------------------//
void game::Screen::updateInput()
{
    InputHandler().update(this->m_Keys);

    static bool isKeyReleased{ false };
    if (this->m_Keys[KEY_W] == false && this->m_Keys[KEY_S] == false)
    {
        isKeyReleased = true;
    }
    if (isKeyReleased == true)
    {
        if (this->m_Keys[KEY_W])
        {
            this->m_MenuSelection = std::clamp(
                --this->m_MenuSelection, 0, this->m_maxMenuSelection
            );
            isKeyReleased = false;
        }
        else if (this->m_Keys[KEY_S])
        {
            this->m_MenuSelection = std::clamp(
                ++this->m_MenuSelection, 0, this->m_maxMenuSelection
            );
            isKeyReleased = false;
        }
    }
}
