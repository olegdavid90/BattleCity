#include "Enemies.hpp"
#include "system/log.hpp"
#include "system/utils.hpp"
#include "game/ResourcesManager.hpp"
#include "game/states/Level.hpp"

#include <algorithm>


//----------------------------------------------------------------------------//
game::Enemies::Enemies(Level* parent_level)
	: m_pParentLevel{ parent_level }
	, m_GameTime    {}
	, m_ActiveCount {}
	, m_maxActive   { 4 }
	, m_LastAdded   {  }
	, m_isTimeToAdd { true }
{
	this->init();
	this->m_GameTimer.start(1);
}

//----------------------------------------------------------------------------//
void game::Enemies::update()
{
	this->m_RespawnTimer.update();
	this->m_FreezeTimer.update();
	this->m_GameTimer.update();

	if (this->m_isTimeToAdd)
	{
		this->m_isTimeToAdd = false;
		this->m_RespawnTimer.start(this->m_RespawnPeriod);

		if (this->m_ActiveCount < this->m_maxActive
			&& this->m_LastAdded < this->m_pEnemyTanks.size())
		{
			if (this->m_LastAdded == 3 || this->m_LastAdded == 10 
				|| this->m_LastAdded == 17)
			{
				this->m_pParentLevel->resetBonus();
				this->m_pEnemyTanks[this->m_LastAdded]->setBonus();
			}
			this->m_pEnemyTanks[this->m_LastAdded]->setLevel(this->m_pParentLevel);
			this->m_pParentLevel->addActiveDynamicObject(
				this->m_pEnemyTanks[this->m_LastAdded].get()
			);

			++this->m_LastAdded;
			++this->m_ActiveCount;
		}
	}
}

//----------------------------------------------------------------------------//
void game::Enemies::init()
{
	this->initTimers();
	this->m_RespawnPeriod = 190 - this->m_pParentLevel->getIndex() * 4;
	this->m_ActionDuration = this->m_RespawnPeriod / 8;
	this->loadTanks();
	this->correctTanksCount();
	this->shuffleTanks();
}

//----------------------------------------------------------------------------//
void game::Enemies::loadTanks()
{
	const unsigned levelIndex = this->m_pParentLevel->getIndex();
	const auto& tanksCount{
		ResourcesManager::getEnemyTanksCount(this->m_pParentLevel->getIndex())
	};
	for (unsigned subtype{}; subtype < tanksCount.size(); ++subtype)
	{
		for (unsigned count{}; count < tanksCount[subtype]; ++count)
		{
			this->m_pEnemyTanks.emplace_back(
				std::make_unique<EnemyTank>(static_cast<EnemyTank::ESubType>(subtype), this)
			);
		}
	}

}

//----------------------------------------------------------------------------//
void game::Enemies::shuffleTanks()
{
	for (unsigned noTank{}; noTank < this->m_pEnemyTanks.size(); ++noTank)
	{
		std::swap(
			this->m_pEnemyTanks[noTank],
			this->m_pEnemyTanks[utils::rand() % this->m_pEnemyTanks.size()]);
	}
}

//----------------------------------------------------------------------------//
void game::Enemies::correctTanksCount()
{
	if (this->m_pEnemyTanks.size() > 20)
	{
		this->m_pEnemyTanks.resize(20);
	}

	while (this->m_pEnemyTanks.size() < 20)
	{
		this->m_pEnemyTanks.emplace_back(
			std::make_unique<EnemyTank>(EnemyTank::ESubType::Regular, this)
		);
	}
}

//----------------------------------------------------------------------------//
void game::Enemies::removeTank()
{
	--this->m_ActiveCount;

	if (this->m_ActiveCount == 0
		&& this->m_LastAdded == this->m_pEnemyTanks.size())
	{
		this->m_pParentLevel->finish();
	}
}

//----------------------------------------------------------------------------//
void game::Enemies::freezeEnemies()
{
	for (auto& pTank : this->m_pEnemyTanks)
	{
		pTank->freeze(true);
	}
	this->m_FreezeTimer.start(10);
}

//----------------------------------------------------------------------------//
void game::Enemies::removeActiveEnemies()
{
	for (auto& pTank : this->m_pEnemyTanks)
	{
		pTank->destroy(false);
	}
}

//----------------------------------------------------------------------------//
game::EActionType game::Enemies::getActionType() const
{
	return this->m_eActionType;
}

//----------------------------------------------------------------------------//
unsigned game::Enemies::getPassiveTanksCount() const
{
	return static_cast<unsigned>(this->m_pEnemyTanks.size() - this->m_LastAdded);
}

//----------------------------------------------------------------------------//
void game::Enemies::initTimers()
{
	this->m_RespawnTimer.setCallback([this] { this->m_isTimeToAdd = true; });
	this->m_GameTimer.setCallback(
		[this]()
		{
			++this->m_GameTime;
			if (this->m_GameTime < this->m_ActionDuration)
			{
				this->m_eActionType = EActionType::RandomMove;
			}
			else if (this->m_GameTime < this->m_ActionDuration * 2)
			{
				this->m_eActionType = EActionType::MoveToPlayer;
			}
			else
			{
				this->m_eActionType = EActionType::MoveToEagle;
			}
			this->m_GameTimer.start(1);
		}
	);
	this->m_FreezeTimer.setCallback(
		[this]()
		{
			for (auto& pTank : this->m_pEnemyTanks)
			{
				pTank->freeze(false);
			}
		}
	);
}
