#pragma once

#include "DynamicObject.hpp"
#include "system/types.hpp"
#include "system/FramesTimer.hpp"
#include "renderer/Animator.hpp"

#include <array>

namespace game {

namespace renderer { class Sprite2D; }

enum class EBulletType
{
	Slow = 0,
	Fast,
	Reinforced
};


class Bullet : public DynamicObject
{
  private:
	enum class EBulletState
	{
		Wait = 0,
		Active,
		Explosion
	};

  public:
	Bullet(class Tank* owner);

	void update()                                    override;
	std::size_t getID() const                        override;
	void render()       const                        override;
	bool isBullet()     const                        override;
	bool isPlayer()     const                        override;
	bool isDisabled()   const                        override;
	void onCollision(const StaticObject* object)     override;

	EBulletType getType() const;
	bool isExplosion()    const;
	void updateType();
	void ResetState();
	bool fire();

  private:
  	void onCollisionWithBullet(const Bullet* bullet) override;
	void onCollisionWithTank(const Tank* tank)       override;


  private:
	std::array<renderer::Sprite2D*, 4>  m_pBulletSprites;
	renderer::Sprite2D*                 m_pExplosionSprite;
	renderer::Animator                  m_ExplosionAnimator;
	tps::Vec2i                          m_ExplosionOffset;
	FramesTimer                         m_ExplosionTimer;						                
	class Tank*                         m_Owner;
	EBulletType                         m_eBulletType;
	EBulletState                        m_eBulletState;
	int                                 m_Speed;

}; /* !class Bullet */


} /* !namespace game */
