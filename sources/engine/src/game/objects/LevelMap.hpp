#pragma once

#include "system/types.hpp"
#include "system/EDirection.hpp"
#include "system/FramesTimer.hpp"
#include "system/SecondsTimer.hpp"
#include "game/objects/StaticObject.hpp"

#include <vector>
#include <memory>
#include <array>

namespace game {


class LevelMap
{
  private:
	typedef EDirection EBorderType;

	static constexpr unsigned EMPTY_TILE[]{
	  0,   1,  12,  13,  24,  25,  26,  27,  38,  39,  50,  51,
	632, 633, 636, 637, 640, 641, 658, 659, 662, 663, 666, 667
	};

	static constexpr unsigned EAGLE_WALL_TILES[]{
		609, 610, 611, 612, 635, 638, 661, 664
	};


  public:
	LevelMap(const class Level* parent_level);
	
	void render() const;
	void renderTrees() const;
	void update();
	void updateCollisionObjects(
		const tps::IntRect& global_bounds,
		std::vector<class StaticObject*>& objects
	) const;
	void concreteWallsEagle();

  private:
	void initBorders();
	void swapEagleWall();
	void initLevelMap(unsigned level_index);
	void correctLevelMap(std::vector<std::string>& level_desctiption);

	
  private:
	std::vector<std::unique_ptr<StaticObject>>    m_pStaticObjects;
	std::vector<std::unique_ptr<StaticObject>>    m_pTrees;
	std::vector<std::unique_ptr<StaticObject>>    m_pEagleWall;
	std::array<std::unique_ptr<StaticObject>, 4>  m_pBorders;
	std::vector<class Water*>                     m_pWaters;
	FramesTimer                                   m_EagleWallTimer;
	unsigned                                      m_swapEagleWallCount;
	bool                                          m_isBreakEagleWall;

}; /* class Map */


}; /* namespace game */
