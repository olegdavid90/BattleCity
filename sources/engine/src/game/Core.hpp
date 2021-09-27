#pragma once

#include "IEngine.hpp"
#include "system/NonCopyable.hpp"
#include "game/states/GameState.hpp"
#include "game/objects/tanks/PlayerTank.hpp"

#include <memory>

namespace game {

enum class EGameState
{
	StartScreen = 0,
	Level,
	Pause,
	GameOver
};


class Core : public IEngine, public NonCopyable
{
  public:
	Core();

	void process() override;

	void exit();
	void free();
	void pause();
	void resume();
	void gameOver();
	void loadConfig();
	void clearConfig();
	void startNewGame();
	void startNextLevel();
	void saveNewHighScore(unsigned high_score);
	bool isRunning()              const;
	bool isPause()                const;
	unsigned getLevelIndex()      const;
	class PlayerTank* getPlayer() const;


  private:
	void render();


  private:
	class MainWindow*            m_MainWindow;
	std::unique_ptr<PlayerTank>  m_pPlayer;
	std::unique_ptr<GameState>   m_pCurrentState;
	std::unique_ptr<GameState>   m_pPauseState;
	EGameState                   m_eGameState;
	unsigned                     m_LevelIndex;
	bool                         m_isRunning;

}; /* !class Core */


} /* !namespace game */
