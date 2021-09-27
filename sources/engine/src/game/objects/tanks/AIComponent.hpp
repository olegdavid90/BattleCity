#pragma once

#include "system/types.hpp"

namespace game {
	
	
class AIComponent
{
  public:
	static void randomMoveCommand(class EnemyTank* tank);
	static void changeDirectionCommand(class EnemyTank* tank);
	static void moveToPlayerCommand(class EnemyTank* tank);
	static void moveToEagleCommand(class EnemyTank* tank);

  private:
	static void check();
	static void moveToObject(
		class EnemyTank* tank, const tps::Vec2i& object_position
	);
		
}; /* !class AIComponent */
	
	
} /* !namespace game */
