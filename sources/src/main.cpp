#include "IEngine.hpp"


int main()
{
	game::IEngine* pGame{ nullptr };

	getGame(pGame);	
	pGame->process();
	freeGame();
}
