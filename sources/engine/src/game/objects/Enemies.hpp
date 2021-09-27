#pragma once

#include "tanks/EnemyTank.hpp"
#include "system/FramesTimer.hpp"
#include "system/SecondsTimer.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace game {

enum class EActionType
{
	RandomMove = 0,
	MoveToPlayer,
	MoveToEagle
};


class Enemies
{
  public:
	Enemies(class Level* parent_level);

	void update();
	void removeTank();
	void freezeEnemies();
	void removeActiveEnemies();
	EActionType getActionType() const;
	unsigned getPassiveTanksCount() const;

  private:
	void init();
	void loadTanks();
	void shuffleTanks();
	void correctTanksCount();
	void initTimers();


  private:
	std::vector<std::unique_ptr<EnemyTank>>  m_pEnemyTanks;
	class Level*                             m_pParentLevel;
	FramesTimer                              m_RespawnTimer;
	SecondsTimer                             m_GameTimer;
	SecondsTimer                             m_FreezeTimer;
	EActionType                              m_eActionType;

	std::uint8_t                             m_GameTime;
	unsigned                                 m_RespawnPeriod;
	unsigned                                 m_ActionDuration;
	unsigned                                 m_ActiveCount;
	unsigned                                 m_maxActive;												  
	unsigned                                 m_LastAdded;
	bool                                     m_isTimeToAdd;

}; /* !class Enemies */


} /* !namespace game */
