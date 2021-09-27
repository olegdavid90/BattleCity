#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "system/NonCopyable.hpp"


namespace game {

namespace renderer { class Texture2D; }


class MainWindow : public sf::RenderWindow, public NonCopyable
{
  public:
	static MainWindow* instance();
	void setGame(class Core* game);
	void pollEvent();
	void setIcon(const class renderer::Texture2D* icon_texture);

  private:
	MainWindow();

	void resizeView();


  private:
	class Core*  m_pGame;

}; /* class MainWindow */

	
} /* !namespace game */
