#pragma once

#include "Tank.hpp"
#include "game/objects/Bullet.hpp"
#include "system/SecondsTimer.hpp"
#include "system/FramesTimer.hpp"
#include "renderer/Animator.hpp"

#include <array>
#include <vector>

namespace game{


static const tps::Vec2i PLAYER_RESPAWN_POSITION{
	GAME_WIDTH / 2 - BLOCK_SIZE * 3, GAME_HEIGHT - BLOCK_SIZE
};


class PlayerTank : public Tank
{
  public:
	enum class ESubType
	{
		Small = 0,
		Light,
		Medium,
		Heavy
	};

  public:
	PlayerTank();

	virtual ~PlayerTank() = default;

	unsigned getSubType() const                   override;
	bool hasShield()      const                   override;
	void render()         const                   override;
	void loadSprites()                            override;
	void update()                                 override;
	void fire()                                   override;
	void stop()                                   override;
	void destroy(bool)                            override;
	void setLevel(Level* parent_level)            override;
	void onCollision(const StaticObject* object)  override;
	void setOrientation(EOrientation orientation) override;

	void addStar();
	void updateInput();
	void setScore(unsigned score);
	void setHealth(unsigned health);
	void setShield(unsigned seconds);
	void setSubType(unsigned subType);
	unsigned getScore() const;

  private:
	void Reset();
	void initTimers();


  protected:
	std::array<std::unique_ptr<Bullet>, 2>  m_pBullets;
	renderer::Sprite2D*                     m_pShieldSprite;
	renderer::Animator                      m_ShieldAnimator;	
	SecondsTimer                            m_ShieldTimer;
	FramesTimer                             m_CooldownTimer;
	FramesTimer                             m_SlippingTimer;
	ESubType                                m_eSubType;
	unsigned                                m_Score;
	unsigned                                m_BulletsCount;
	unsigned                                m_BulletIndex;
	bool                                    m_hasShield;
	bool                                    m_isCooldownTime;
	bool                                    m_onIce;
	bool                                    m_isSlipping;

}; /* !class Tank */


} /* !namespace game */
