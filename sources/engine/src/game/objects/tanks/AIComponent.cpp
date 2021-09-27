#include "AIComponent.hpp"
#include "game/states/Level.hpp"
#include "game/objects/tanks/EnemyTank.hpp"
#include "system/log.hpp"
#include "system/utils.hpp"


//----------------------------------------------------------------------------//
void game::AIComponent::randomMoveCommand(EnemyTank* tank)
{
	if (tank->getPosition().x % BLOCK_SIZE == 0
		&& tank->getPosition().y % BLOCK_SIZE == 0
		&& utils::rand() % 16 == 0)
	{
		tank->changeScenario();
	}
	else if (tank->frontTileIsBarrier() && utils::rand() % 32 == 0)
	{
		if (utils::rand() % 2 == 0)
		{
			tank->invertDirection();
		}
		else
		{
			tank->setCommand(&AIComponent::changeDirectionCommand);
		}
	}
}

//----------------------------------------------------------------------------//
void game::AIComponent::changeDirectionCommand(EnemyTank* tank)
{
	if (utils::rand() % 2 == 0)
	{
		tank->changeScenario();
	}
	else if (utils::rand() % 2 == 0)
	{
		tank->rotateClockwise();
	}
	else
	{
		tank->rotateAntiClockwise();
	}
}

//----------------------------------------------------------------------------//
void game::AIComponent::moveToObject(
	EnemyTank* tank, const tps::Vec2i& object_position
)
{
	if (tank->frontTileIsBarrier())
	{
		if (utils::rand() % 2)
		{
			if (tank->getPosition().y < object_position.y
				&& tank->getOrientation() != Tank::EOrientation::Bottom)
			{
				tank->setOrientation(Tank::EOrientation::Bottom);
			}
			else if (tank->getPosition().y > object_position.y
				&& tank->getOrientation() != Tank::EOrientation::Top)
			{
				tank->setOrientation(Tank::EOrientation::Top);
			}
			else if (tank->getPosition().x < object_position.x
				&& tank->getOrientation() != Tank::EOrientation::Right)
			{
				tank->setOrientation(Tank::EOrientation::Right);
			}
			else if (tank->getPosition().x > object_position.x
				&& tank->getOrientation() != Tank::EOrientation::Left)
			{
				tank->setOrientation(Tank::EOrientation::Left);
			}
		}
		else if (utils::rand() % 2)
		{
			tank->setRandomDirection();
		}
		else
		{
			tank->setCommand(&AIComponent::changeDirectionCommand);
		}
	}
}

//----------------------------------------------------------------------------//
void game::AIComponent::moveToPlayerCommand(EnemyTank* tank)
{
	moveToObject(tank, tank->getParentLevel()->getPlayerPosition());
}

//----------------------------------------------------------------------------//
void game::AIComponent::moveToEagleCommand(EnemyTank* tank)
{
	moveToObject(tank, EAGLE_POSITION);
}
