#pragma once


namespace game     {
namespace renderer {

class Animator
{
  public:
	Animator(const class Sprite2D* pSprite);

	unsigned getCurrentTile() const;
	unsigned getTotalDuration() const;
	void update();
	void reset(const class Sprite2D* pSprite);
	void reset();


  private:
	const class Sprite2D*  m_pSprite;
	unsigned               m_CurrentTileNumber;
	unsigned               m_DurationLeft;
	unsigned               m_TilesCount;
	unsigned               m_TotalDuration;

}; /* !class Animator */


} /* !namespace render_engine */
} /* !namespace game */
