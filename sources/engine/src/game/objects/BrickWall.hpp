#pragma once

#include "StaticObject.hpp"
#include "system/types.hpp"
#include "system/EDirection.hpp"

#include <array>

namespace game {

namespace renderer { class Sprite2D; }


class BrickWall : public StaticObject
{
  private:
	enum class EWallState {
		All = 0,
		Top,
		Bottom,
		Left,
		Right,
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		Destroyed,
	};

  public:
	BrickWall(const tps::Vec2i& position);

	tps::IntRect getBoundsForBullet() const override;
	void render()                     const override;
	bool isDestroyed()                const override;
	void onCollision(DynamicObject* object) override;

  private:
	void updateWallState(EDirection direction);


  private:
	std::array<renderer::Sprite2D*, 10>  m_pSprites;
	EWallState                           m_eWallState;
};


} /* !namespace game */
