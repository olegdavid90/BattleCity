#pragma once

#include "StaticObject.hpp"
#include "system/types.hpp"
#include "system/FramesTimer.hpp"

namespace game {

namespace renderer { class Sprite2D; }


class Bonus : public StaticObject
{
  private:
	enum class EBonusType
	{
		Helmet = 0,
		Clock,
		Star,
		Grenade,
		Shovel,
		Tank
	};


  public:
	Bonus(class Level* parent_level);

	void update()                                          override;
	void render()                                    const override;
	bool collides(const class DynamicObject* object) const override;
	void onCollision(class DynamicObject* player_tank)     override;

	static tps::Vec2i getRandomBonusPosition();


  private:
	  class Level*         m_pParentLevel;
	  renderer::Sprite2D*  m_pSprite;
	  FramesTimer          m_ShowTimer;
	  EBonusType           m_eBonusType;
	  bool                 m_isShowTime;

}; /* !class Bonus */


} /* !namespace game */
