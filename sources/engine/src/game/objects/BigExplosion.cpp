#include "BigExplosion.hpp"
#include "system/log.hpp"
#include "renderer/Sprite2D.hpp"
#include "game/ResourcesManager.hpp"

#include <algorithm>


//----------------------------------------------------------------------------//
game::BigExplosion::BigExplosion(const tps::Vec2i& position, unsigned score)
	: StaticObject(
		position - tps::Vec2i{ BLOCK_SIZE, BLOCK_SIZE },
		tps::Vec2i{ BLOCK_SIZE * 4, BLOCK_SIZE * 4 }
	)
	, m_pExplosionSprite { ResourcesManager::getSprite2D("bigExplosion") }
	, m_pScoreSprite     { ResourcesManager::getSprite2D("score_" + std::to_string(score)) }
	, m_ExplosionAnimator{ this->m_pExplosionSprite }
	, m_ExplosionTimeLeft{ this->m_ExplosionAnimator.getTotalDuration() }
	, m_ScoreTimeLeft    { this->m_pScoreSprite->getTileDuration(0) }
{
	this->m_pExplosionSprite->setSize(this->getSize());
	this->m_pScoreSprite->setSize(this->getSize());
}

//----------------------------------------------------------------------------//
void game::BigExplosion::render() const
{
	if (this->m_ExplosionTimeLeft > 0)
	{
		this->m_pExplosionSprite->render(
			this->getPosition(), this->m_ExplosionAnimator.getCurrentTile()
		);
	}
	else if (this->m_ScoreTimeLeft > 0)
	{
		this->m_pScoreSprite->render(this->getPosition());
	}
}

//----------------------------------------------------------------------------//
void game::BigExplosion::update()
{
	if (this->m_ExplosionTimeLeft > 0)
	{
		--this->m_ExplosionTimeLeft;
		this->m_ExplosionAnimator.update();
	}
	else if (this->m_ScoreTimeLeft > 0)
	{
		--this->m_ScoreTimeLeft;
	}
	else
	{
		this->m_eObjectState = EObjectState::Disabled;
	}
}
