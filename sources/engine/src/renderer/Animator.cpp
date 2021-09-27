#include "Animator.hpp"
#include "Sprite2D.hpp"

namespace game::renderer {

//----------------------------------------------------------------------------//
Animator::Animator(const Sprite2D* pSprite)
	: m_pSprite          { pSprite }
	, m_CurrentTileNumber{}
	, m_DurationLeft     { this->m_pSprite->getTileDuration(this->m_CurrentTileNumber) }
	, m_TilesCount       { this->m_pSprite->getTilesCount() }
	, m_TotalDuration    {}
{
	for (unsigned noFrame{}; noFrame < this->m_pSprite->getTilesCount(); ++noFrame)
	{
		this->m_TotalDuration += this->m_pSprite->getTileDuration(noFrame);
	}
}

//----------------------------------------------------------------------------//
unsigned Animator::getCurrentTile() const
{
	return this->m_CurrentTileNumber;
}

//----------------------------------------------------------------------------//
void Animator::update()
{
	--this->m_DurationLeft;

	if (this->m_DurationLeft == 0)
	{
		++this->m_CurrentTileNumber;

		if (this->m_CurrentTileNumber >= this->m_TilesCount)
		{
			this->m_CurrentTileNumber = 0;
		}

		this->m_DurationLeft = this->m_pSprite->getTileDuration(this->m_CurrentTileNumber);
	}
}

//----------------------------------------------------------------------------//
unsigned Animator::getTotalDuration() const
{
	return this->m_TotalDuration;
}

//----------------------------------------------------------------------------//
void Animator::reset(const Sprite2D* pSprite)
{
	if (this->m_pSprite != pSprite)
	{
		this->m_pSprite = pSprite;
		this->reset();
	}
}

//----------------------------------------------------------------------------//
void Animator::reset()
{
	this->m_CurrentTileNumber = 0;
	this->m_TilesCount = this->m_pSprite->getTilesCount();
	this->m_DurationLeft =
		this->m_pSprite->getTileDuration(this->m_CurrentTileNumber);
}


} /* !namespace game::render_engine */
