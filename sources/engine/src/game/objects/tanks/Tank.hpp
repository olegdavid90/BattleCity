#pragma once

#include "game/objects/DynamicObject.hpp"
#include "system/SecondsTimer.hpp"
#include "renderer/Animator.hpp"

#include <memory>
#include <array>

namespace game{

namespace renderer { class Sprite2D; }

enum class ETankType
{
	Player = 0,
	Enemy,
};


class Tank : public DynamicObject
{
  protected:
	enum class ETankState
	{
		Respawn = 0,
		Active,
		Destroyed
	};

  public:
	Tank(ETankType type, const tps::Vec2i& position);
	virtual ~Tank() = default;

	virtual void setLevel(class Level* parent_level) = 0;
	virtual void destroy(bool desyroyed_with_bullet) = 0;
	virtual void fire()                              = 0;
	virtual void loadSprites()                       = 0;
	virtual unsigned getSubType() const              = 0;

	virtual bool hasShield() const;

	void setPosition(const tps::Vec2i& position)     override;
	void setOrientation(EOrientation orientation)    override;
	void onCollision(const StaticObject* object)     override;
	bool isTank()       const                        override;
	bool isPlayer()     const                        override;
	bool isDisabled()   const                        override;
	std::size_t getID() const                        override;

	int getHealth()               const;
	const Level* getParentLevel() const;
	bool isRespawning()           const;
	bool frontTileIsBarrier()     const;
	void freeze(bool is_frozen);

  private:
  	void onCollisionWithBullet(const Bullet* bullet) override;
	void onCollisionWithTank(const Tank* tank)       override;
	void alignmentPosition();


  protected:
	std::array<renderer::Sprite2D*, 4>  m_pTankSprites;
	renderer::Sprite2D*                 m_pRespawnSprite;
	renderer::Animator                  m_TankAnimator;
	SecondsTimer                        m_RespawnTimer;
	class Level*                        m_pParentLevel;
	ETankType                           m_eTankType;
	ETankState                          m_eTankState;
	tps::Vec2i                          m_oldPosition;
	unsigned                            m_FramesCount;
	int                                 m_Health;
	bool                                m_hasBarrierAhead;
	bool                                m_isFrozen;

}; /* !class Tank */


} /* !namespace game */
