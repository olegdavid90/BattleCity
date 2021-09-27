#pragma once

#include "IEngine.hpp"
#include "system/types.hpp"
#include "system/EDirection.hpp"
#include "system/SecondsTimer.hpp"
#include "renderer/Text2D.hpp"
#include "game/PhysicsEngine.hpp"
#include "game/states/GameState.hpp"
#include "game/states/LevelState.hpp"
#include "game/objects/LevelMap.hpp"
#include "game/objects/Enemies.hpp"
#include "game/objects/LevelGUI.hpp"
#include "game/objects/StaticObject.hpp"

#include <memory>
#include <vector>
#include <string>
#include <array>

namespace game {


static const tps::Vec2i EAGLE_POSITION{
	GAME_WIDTH / 2 + BLOCK_SIZE, GAME_HEIGHT - BLOCK_SIZE
};


class Level : public GameState
{
  private:
	typedef EDirection EBorderType;

	enum class ELevelState
	{
		Start = 0,
		Active,
		Finish,
	};


  public:
	Level(class Core* pCore);
	
	void updateInput()  override;
	void initText()     override;
	void update()       override;
	void render() const override;
	
	void run();
	void finish();
	void nextLevel();
	void resetBonus();
	void createBonus();
	void startGameOver();
	void finishGameOver();
	void freezeEnemies();
	void concreteWallsEagle();
	void removeActiveEnemies();
	void setState(ELevelState state);
	void addActiveDynamicObject(DynamicObject* pGame_object);
	void setExplosion(const tps::Vec2i& position, unsigned score = 0);
	bool isGameOver()                                          const;
	int getPlayerHealth()                                      const;
	unsigned getIndex()                                        const;
	unsigned getPlayerScore()                                  const;
	unsigned getPlayerSubType()                                const;
	const tps::Vec2i& getPlayerPosition()                      const;
	const std::array<unsigned, 5>& getDestroyedEnemiesCounts() const;
	std::vector<class StaticObject*> getCollisionObjects(
		const tps::IntRect& global_bounds
	)                                                          const;

  private:
	void saveConfig();


  private:
	PhysicsEngine                               m_PhysicsEngine;
	std::array<unsigned, 5>                     m_DestroyedEnemiesCounts;
	std::unique_ptr<LevelMap>                   m_pLevelMap;
	std::unique_ptr<Enemies>                    m_pEnemies;
	std::unique_ptr<LevelGUI>                   m_pLevelGUI;
	std::unique_ptr<StaticObject>               m_pBonus;
	std::unique_ptr<StaticObject>               m_pEagle;
	std::unique_ptr<LevelState>                 m_pLevelStart;
	std::unique_ptr<LevelState>                 m_pLevelFinish;
	std::unique_ptr<renderer::Text2D>           m_pGameOverText;
	std::vector<std::unique_ptr<StaticObject>>  m_BigExplosions;
	class PlayerTank*                           m_pPlayer;
	ELevelState                                 m_eLevelState;													 
	SecondsTimer                                m_FinishTimer;
												
	unsigned                                    m_FramesCount;
	bool                                        m_isGameOver;
													     
}; /* !class Level */


} /* !namespace game */
