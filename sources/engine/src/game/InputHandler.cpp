#include "InputHandler.hpp"

#include <SFML/Window/Keyboard.hpp>


//----------------------------------------------------------------------------//
void game::InputHandler::update(bool (&keys)[3])
{
	keys[KEY_ENTER] = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
	keys[KEY_W]     = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	keys[KEY_S]     = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
}

//----------------------------------------------------------------------------//
void game::InputHandler::update(bool (&keys)[6])
{
	keys[KEY_ENTER] = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
	keys[KEY_W]     = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	keys[KEY_S]     = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	keys[KEY_A]     = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	keys[KEY_D]     = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	keys[KEY_J]     = sf::Keyboard::isKeyPressed(sf::Keyboard::J);
}
