#pragma once

#include "Tank.hpp"
#include "game/objects/Bullet.hpp"
#include "system/FramesTimer.hpp"
#include "renderer/Animator.hpp"

#include <memory>
#include <array>


namespace game {


static const tps::Vec2i ENEMY_RESPAWN_POSITIONS[3]{
	tps::Vec2i{ BLOCK_SIZE * 2,              BLOCK_SIZE },
	tps::Vec2i{ GAME_WIDTH / 2 + BLOCK_SIZE, BLOCK_SIZE },
	tps::Vec2i{ GAME_WIDTH,                  BLOCK_SIZE }
};


class EnemyTank : public Tank
{
  private:
	typedef void (*Command)(EnemyTank*);

  public:
    enum class ESubType
    {
        Regular = 0,
        HighSpeed,
        RapidFire,
        Armored
    };

  public:
	EnemyTank(ESubType subtype, class Enemies* owner);

    unsigned getSubType() const              override;
	void render()         const              override;
	void fire()                              override;
	void update()                            override;
	void loadSprites()                       override;
	void destroy(bool desyroyed_with_bullet) override;
	void setLevel(Level* parent_level)       override;

	void setBonus();
	void changeScenario();
	void invertDirection();
	void rotateClockwise();
	void rotateAntiClockwise();
	void setRandomDirection();
	void setCommand(const Command& command);


  private:
	std::array<renderer::Sprite2D*, 4>  m_pBonusTankSprites;
	std::unique_ptr<Bullet>             m_pBullet;
	Command                             m_Command;
	FramesTimer                         m_BonusTankShowTimer;
	ESubType                            m_eSubType;
	class Enemies*                      m_Owner;
	unsigned                            m_SpeedMask;
	bool                                m_isBonusTankShowTime;
	bool                                m_hasBonus;

}; /* !class Tank */


} /* !namespace game */
