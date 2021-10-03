#include "MainWindow.hpp"
#include "IEngine.hpp"
#include "game/Core.hpp"
#include "system/log.hpp"
#include "renderer/Texture2D.hpp"

#include <SFML/Window/Event.hpp>
#include <memory>

//----------------------------------------------------------------------------//
game::MainWindow* game::MainWindow::instance()
{
    static MainWindow window{};
    return std::addressof(window);
}

//----------------------------------------------------------------------------//
void game::MainWindow::setGame(Core* game)
{
    this->m_pGame = game;
}

//----------------------------------------------------------------------------//
game::MainWindow::MainWindow()
{
    sf::RenderWindow::create(
        sf::VideoMode{
            static_cast<unsigned>(WINDOW_WIDTH  * WINDOW_SCALE),
            static_cast<unsigned>(WINDOW_HEIGHT * WINDOW_SCALE)
        },
        GAME_TITLE,
        sf::Style::Default
    );

    sf::RenderWindow::setKeyRepeatEnabled(false);
    this->resizeView();
}

//----------------------------------------------------------------------------//
void game::MainWindow::pollEvent()
{
    sf::Event event{};

    while (sf::RenderWindow::pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::KeyPressed:
            break;
        case sf::Event::Resized:
            this->resizeView();
            break;
        case sf::Event::LostFocus:
            this->m_pGame->pause();
            break;
        /* case sf::Event::GainedFocus:
            this->m_pGame->resume();
            break; */
        case sf::Event::TextEntered:
            if (event.text.unicode == 27U) // Escape
            {
                this->m_pGame->exit();
            }
            else if (event.key.code == 32U) // Space
            {
                this->m_pGame->isPause() ? this->m_pGame->resume() :
                                           this->m_pGame->pause()  ;
            }
            break;
        case sf::Event::Closed:
            this->m_pGame->exit();
        }
    }
}

//----------------------------------------------------------------------------//
void game::MainWindow::setIcon(const renderer::Texture2D* icon_texture)
{
    if (icon_texture->loaded())
    {
        sf::RenderWindow::setIcon(
            icon_texture->getSize().x,
            icon_texture->getSize().y,
            icon_texture->copyToImage().getPixelsPtr()
        );
    }
}

//----------------------------------------------------------------------------//
void game::MainWindow::resizeView()
{
    if (sf::RenderWindow::getSize().x < WINDOW_WIDTH)
    {
        sf::RenderWindow::setSize({ WINDOW_WIDTH, sf::RenderWindow::getSize().y });
    }
    if (sf::RenderWindow::getSize().y < WINDOW_HEIGHT)
    {
        sf::RenderWindow::setSize({ sf::RenderWindow::getSize().x, WINDOW_HEIGHT });
    }

    const sf::Vector2f currentWindowSize{
        static_cast<sf::Vector2f>(sf::RenderWindow::getSize())
    };
    const float currentAspectRatio{ currentWindowSize.x / currentWindowSize.y };

    float currentScale{};
    float offsetLeft{};
    float offsetTop{};

    if (currentAspectRatio > WINDOW_ASPECT_RATIO)
    {
        currentScale = currentWindowSize.y / WINDOW_HEIGHT;
        offsetLeft = (1.F - WINDOW_WIDTH * currentScale / currentWindowSize.x) / 2.F;
    }
    else
    {
        currentScale = currentWindowSize.x / WINDOW_WIDTH;
        offsetTop = (1.F - WINDOW_HEIGHT * currentScale / currentWindowSize.y) / 2.F;
    }

    sf::View currentView({ 0.F, 0.F, currentWindowSize.x, currentWindowSize.y });
    currentView.setViewport({ offsetLeft, offsetTop, currentScale, currentScale });
    sf::RenderWindow::setView(currentView);
}
