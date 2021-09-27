#pragma once

#include "system/types.hpp"
#include "system/SecondsTimer.hpp"
#include "game/ResourcesManager.hpp"

#include <memory>

namespace game {

namespace renderer {
	class Sprite2D;
	class Text2D;
} /* !namespace renderer */


class LevelState
{
  protected:
	enum class ELevelStateType
	{
		ShowInfo = 0,
		ShowBorder
	};


  public:
	LevelState(class Level* parent_level, const tps::Vec2i& border_size);
	virtual ~LevelState() = default;

	virtual void render() const = 0;
	virtual void update()       = 0;
	virtual void initText()     = 0;
	
	
  protected:
	class Level*         m_pParentLevel;
	tps::Vec2i           m_BorderSize;
	renderer::Sprite2D*  m_pBorder;
	SecondsTimer         m_StateTimer;
	ELevelStateType      m_eStateType;

}; /* !class LevelState */


inline LevelState::LevelState(Level* parent_level, const tps::Vec2i& border_size)
	: m_pParentLevel{ parent_level }
	, m_BorderSize  { border_size }
	, m_pBorder     { ResourcesManager::getSprite2D("border") }
	, m_eStateType  { ELevelStateType::ShowInfo }
{ }


} /* !namespase game */
