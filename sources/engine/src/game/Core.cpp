#include "Core.hpp"
#include "game/objects/tanks/PlayerTank.hpp"
#include "system/log.hpp"
#include "system/utils.hpp"
#include "renderer/MainWindow.hpp"
#include "ResourcesManager.hpp"
#include "game/states/Level.hpp"
#include "game/states/PauseScreen.hpp"
#include "game/states/GameOverScreen.hpp"
#include "game/states/StartScreen.hpp"

#include <string>
#include <sstream>
#include <algorithm>
#include <sstream>

using namespace std::chrono_literals;


static game::Core* g_pCore{ nullptr };

//----------------------------------------------------------------------------//
bool getGame(game::IEngine*& pIEngine)
{
    if (g_pCore == nullptr)
    {
        g_pCore = new game::Core();
        pIEngine = static_cast<game::IEngine*>(g_pCore);
        return true;
    }
    else
    {
        pIEngine = static_cast<game::IEngine*>(g_pCore);
        return false;
    }
}

//----------------------------------------------------------------------------//
void freeGame()
{
    if (g_pCore != nullptr)
    {
        g_pCore->free();
        delete g_pCore;
        g_pCore = nullptr;
    }
}
    
//----------------------------------------------------------------------------//
game::Core::Core()
    : m_MainWindow{ MainWindow::instance() }
    , m_eGameState{ EGameState::StartScreen }
    , m_LevelIndex{ 1 }
    , m_isRunning { true }
{
    ResourcesManager::load("resources.json");
    this->m_MainWindow->setGame(this);
    this->m_MainWindow->setIcon(ResourcesManager::getTexture2D("icon"));
    this->m_pPlayer       = std::make_unique<PlayerTank>();
    this->m_pCurrentState = std::make_unique<StartScreen>(this);
    this->m_pPauseState   = std::make_unique<PauseScreen>(this);
}

//----------------------------------------------------------------------------//
void game::Core::exit()
{
    const unsigned playerScore{ ResourcesManager::getSavedValue("player_score") };
    const unsigned highScore  { ResourcesManager::getSavedValue("high_score") };

    if (playerScore > highScore)
    {
        this->saveNewHighScore(playerScore);
    }

    this->m_isRunning = false;
}

//----------------------------------------------------------------------------//
void game::Core::pause()
{
    if (this->m_eGameState == EGameState::Level)
    {
        this->m_eGameState = EGameState::Pause;
        std::swap(this->m_pCurrentState, this->m_pPauseState);
    }
}

//----------------------------------------------------------------------------//
void game::Core::resume()
{
    this->m_eGameState = EGameState::Level;
    std::swap(this->m_pCurrentState, this->m_pPauseState);
}

//----------------------------------------------------------------------------//
void game::Core::gameOver()
{
    this->m_eGameState = EGameState::GameOver;
    this->m_pCurrentState.reset();
    this->m_pCurrentState = std::make_unique<GameOverScreen>(this);
}

//----------------------------------------------------------------------------//
bool game::Core::isRunning() const
{
    return this->m_isRunning;
}

//----------------------------------------------------------------------------//
bool game::Core::isPause() const
{
    return (this->m_eGameState == EGameState::Pause);
}

//----------------------------------------------------------------------------//
unsigned game::Core::getLevelIndex() const
{
    return this->m_LevelIndex;
}

//----------------------------------------------------------------------------//
game::PlayerTank* game::Core::getPlayer() const
{
    return this->m_pPlayer.get();
}

//----------------------------------------------------------------------------//
void game::Core::startNewGame()
{
    this->clearConfig();
    this->m_LevelIndex = 1;
    this->m_pPlayer.reset();
    this->m_pCurrentState.reset();
    this->m_pPauseState.reset();

    this->m_pPlayer       = std::make_unique<PlayerTank>();
    this->m_pCurrentState = std::make_unique<Level>(this);
    this->m_pPauseState   = std::make_unique<PauseScreen>(this);
    this->m_eGameState    = EGameState::Level;
}

//----------------------------------------------------------------------------//
void game::Core::startNextLevel()
{
    ++this->m_LevelIndex;
    this->m_pCurrentState.reset();
    this->m_pCurrentState = std::make_unique<Level>(this);
    this->m_eGameState = EGameState::Level;
}

//----------------------------------------------------------------------------//
void game::Core::process()
{
    const auto frameDuration{ 1'000'000'000ns / FPS };
    auto       lastTime     { std::chrono::steady_clock::now() };
    auto       duration     { 0ns };

    while (this->isRunning())
    {
        if (duration < frameDuration)
        {
            auto currentTime{ std::chrono::steady_clock::now() };
            duration += currentTime - lastTime;
            lastTime = currentTime;

            this->m_MainWindow->pollEvent();
            this->m_pCurrentState->updateInput();
            this->render();
        }
        else
        {
            this->m_pCurrentState->update();
            duration = 0ns;
        }
    }
}

//----------------------------------------------------------------------------//
void game::Core::render()
{
    this->m_MainWindow->clear();
    this->m_pCurrentState->render();
    this->m_MainWindow->display();
}

//----------------------------------------------------------------------------//
void game::Core::free()
{
    ResourcesManager::free();
}

//----------------------------------------------------------------------------//
void game::Core::clearConfig()
{
    ResourcesManager::saveConfig(
        "\nhigh_score " + std::to_string(ResourcesManager::getSavedValue("high_score"))
    );
}

//----------------------------------------------------------------------------//
void game::Core::loadConfig()
{
    unsigned playerHealth { ResourcesManager::getSavedValue("player_health") };
    unsigned playerSubtype{ ResourcesManager::getSavedValue("player_subtype") };

    this->m_LevelIndex = ResourcesManager::getSavedValue("last_level");
    this->m_pPlayer->setHealth( (playerHealth == 0) ? 2 : playerHealth);
    this->m_pPlayer->setSubType((playerSubtype > 3) ? 0 : playerSubtype);
    this->m_pPlayer->setScore(ResourcesManager::getSavedValue("player_score"));
}

//----------------------------------------------------------------------------//
void game::Core::saveNewHighScore(unsigned high_score)
{
    std::ostringstream osstrm{};
    osstrm << "\nhigh_score      " << high_score
           << "\nlast_level      " << ResourcesManager::getSavedValue("last_level")
           << "\nplayer_subtype  " << ResourcesManager::getSavedValue("player_subtype")
           << "\nplayer_health   " << ResourcesManager::getSavedValue("player_health")
           << "\nplayer_score    " << ResourcesManager::getSavedValue("player_score")
           << '\n';

    ResourcesManager::saveConfig(osstrm.str());
}
