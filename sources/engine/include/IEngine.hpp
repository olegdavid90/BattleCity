#pragma once

#include <chrono>

namespace game {


constexpr auto FPS                     { 60 };
constexpr auto BLOCKS_PER_GAME_WIDTH   { 26 };
constexpr auto BLOCKS_PER_GAME_HEIGHT  { 26 };
constexpr auto BLOCKS_PER_WINDOW_WIDTH { BLOCKS_PER_GAME_WIDTH  + 6 };
constexpr auto BLOCKS_PER_WINDOW_HEIGHT{ BLOCKS_PER_GAME_HEIGHT + 2 };
constexpr auto BLOCK_SIZE              { 8 };
constexpr auto GAME_WIDTH              { BLOCKS_PER_GAME_WIDTH    * BLOCK_SIZE };
constexpr auto GAME_HEIGHT             { BLOCKS_PER_GAME_HEIGHT   * BLOCK_SIZE };
constexpr auto WINDOW_WIDTH            { BLOCKS_PER_WINDOW_WIDTH  * BLOCK_SIZE };
constexpr auto WINDOW_HEIGHT           { BLOCKS_PER_WINDOW_HEIGHT * BLOCK_SIZE };
constexpr auto WINDOW_SCALE            { 2.F }; // FIXME
constexpr auto WINDOW_ASPECT_RATIO     { static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT };
constexpr auto GAME_TITLE              { "BATTLE CITY" };

	
class IEngine
{
  public:
	virtual ~IEngine() = default;

	virtual void process() = 0;

}; /* !interface IEngine */


} /* !namespace game */

extern bool getGame(game::IEngine*& pIEngine);
extern void freeGame();
