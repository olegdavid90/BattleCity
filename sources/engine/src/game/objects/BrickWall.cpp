#include "BrickWall.hpp"
#include "DynamicObject.hpp"
#include "system/log.hpp"
#include "renderer/Sprite2D.hpp"
#include "game/ResourcesManager.hpp"

namespace game {

//----------------------------------------------------------------------------//
BrickWall::BrickWall(const tps::Vec2i& position)
	: StaticObject(position, tps::Vec2i{ BLOCK_SIZE, BLOCK_SIZE })
	, m_pSprites{
		ResourcesManager::getSprite2D("brickWall_All"),                  
		ResourcesManager::getSprite2D("brickWall_Top"),
		ResourcesManager::getSprite2D("brickWall_Bottom"),
		ResourcesManager::getSprite2D("brickWall_Left"),
		ResourcesManager::getSprite2D("brickWall_Right"),
		ResourcesManager::getSprite2D("brickWall_TopLeft"),
		ResourcesManager::getSprite2D("brickWall_TopRight"),
		ResourcesManager::getSprite2D("brickWall_BottomLeft"),
		ResourcesManager::getSprite2D("brickWall_BottomRight"),
		ResourcesManager::getSprite2D("empty")
	}
	, m_eWallState{ EWallState::All }
{
	for (auto currentSprite : this->m_pSprites)
	{
		currentSprite->setSize(this->getSize());
	}
}

//----------------------------------------------------------------------------//
void BrickWall::updateWallState(EDirection direction)
{
	switch (this->m_eWallState)
	{
	case EWallState::All:
		switch (direction)
		{
		case EDirection::Left:
			this->m_eWallState = EWallState::Right;
			return;
		case EDirection::Right:
			this->m_eWallState = EWallState::Left;
			return;
		case EDirection::Top:
			this->m_eWallState = EWallState::Bottom;
			return;
		case EDirection::Bottom:
			this->m_eWallState = EWallState::Top;
			return;
		}
	case EWallState::Top:
		switch (direction)
		{
		case EDirection::Left:
			this->m_eWallState = EWallState::TopRight;
			return;
		case EDirection::Right:
			this->m_eWallState = EWallState::TopLeft;
			return;
		default:
			this->m_eWallState = EWallState::Destroyed;
			return;
		}
	case EWallState::Bottom:
		switch (direction)
		{
		case EDirection::Left:
			this->m_eWallState = EWallState::BottomRight;
			return;
		case EDirection::Right:
			this->m_eWallState = EWallState::BottomLeft;
			return;
		default:
			this->m_eWallState = EWallState::Destroyed;
			return;
		}
	case EWallState::Left:
		switch (direction)
		{
		case EDirection::Top:
			this->m_eWallState = EWallState::BottomLeft;
			return;
		case EDirection::Bottom:
			this->m_eWallState = EWallState::TopLeft;
			return;
		default:
			this->m_eWallState = EWallState::Destroyed;
			return;
		}
	case EWallState::Right:
		switch (direction)
		{
		case EDirection::Top:
			this->m_eWallState = EWallState::BottomRight;
			return;
		case EDirection::Bottom:
			this->m_eWallState = EWallState::TopRight;
			return;
		default:
			this->m_eWallState = EWallState::Destroyed;
			return;
		}
	default:
		this->m_eWallState = EWallState::Destroyed;
	}
}

//----------------------------------------------------------------------------//
tps::IntRect BrickWall::getBoundsForBullet() const
{
	tps::Vec2i positionOffset{};
	tps::Vec2i newSize{ this->getSize() };

	switch (this->m_eWallState)
	{
	case EWallState::Top:
		newSize        = tps::Vec2i{ BLOCK_SIZE, BLOCK_SIZE / 2 };
		break;
	case EWallState::Bottom:
		positionOffset = tps::Vec2i{ 0,          BLOCK_SIZE / 2 };
		newSize        = tps::Vec2i{ BLOCK_SIZE, BLOCK_SIZE / 2 };
		break;
	case EWallState::Left:
		newSize        = tps::Vec2i{ BLOCK_SIZE / 2, BLOCK_SIZE };
		break;
	case EWallState::Right:
		positionOffset = tps::Vec2i{ BLOCK_SIZE / 2, 0 };
		newSize        = tps::Vec2i{ BLOCK_SIZE / 2, BLOCK_SIZE };
		break;
	case EWallState::TopLeft:
		newSize        = tps::Vec2i{ BLOCK_SIZE / 2, BLOCK_SIZE / 2 };
		break;
	case EWallState::TopRight:
		positionOffset = tps::Vec2i{ BLOCK_SIZE / 2, 0 };
		newSize        = tps::Vec2i{ BLOCK_SIZE / 2, BLOCK_SIZE / 2 };
		break;
	case EWallState::BottomLeft:
		positionOffset = tps::Vec2i{ 0,              BLOCK_SIZE / 2 };
		newSize        = tps::Vec2i{ BLOCK_SIZE / 2, BLOCK_SIZE / 2 };
		break;
	case EWallState::BottomRight:
		positionOffset = tps::Vec2i{ BLOCK_SIZE / 2, BLOCK_SIZE / 2 };
		newSize        = tps::Vec2i{ BLOCK_SIZE / 2, BLOCK_SIZE / 2 };
		break;
	case EWallState::Destroyed:
	default:
		return this->getGlobalBounds();
	}

	return tps::IntRect{ this->getPosition() + positionOffset, newSize };
}

//----------------------------------------------------------------------------//
void BrickWall::render() const
{
	this->m_pSprites[static_cast<unsigned>(this->m_eWallState)]->render(
		this->getPosition()
	);
}

//----------------------------------------------------------------------------//
void BrickWall::onCollision(DynamicObject* object)
{
	if (object->isTank())
	{ return; }

	const EDirection collisionDirection{
		(object->getOrientation() == EDirection::Left)  ? EDirection::Right  :
		(object->getOrientation() == EDirection::Right) ? EDirection::Left   :
		(object->getOrientation() == EDirection::Top)   ? EDirection::Bottom :
														  EDirection::Top
	};

	this->updateWallState(collisionDirection);
	if (this->m_eWallState == EWallState::Destroyed)
	{
		this->m_GlobalBounds = tps::IntRect{};
	}
}

//----------------------------------------------------------------------------//
bool BrickWall::isDestroyed() const
{
	return (this->m_eWallState == EWallState::Destroyed);
}


} /* !namespace game */
