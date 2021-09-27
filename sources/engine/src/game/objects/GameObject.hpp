
#pragma once

#include "IEngine.hpp"
#include "system/types.hpp"

#include <typeinfo>


namespace game {


class GameObject
{
  public:
	enum class EObjectState {
		Enabled = 0,
		Disabled,
		Destroyed
	};

	GameObject(const tps::Vec2i& position, const tps::Vec2i& size);
	virtual ~GameObject() = default;

	virtual void render() const = 0;

	virtual void update();
	virtual void setPosition(const tps::Vec2i& position);
	virtual void setPosition(int x, int y);
	virtual void setSize(tps::Vec2i size);
	virtual void setSize(int width, int height);
	virtual bool isDestroyed() const;
	virtual bool isDisabled() const;

	const tps::Vec2i& getPosition() const;
	const tps::Vec2i& getSize() const;


  protected:
	tps::Vec2i    m_Position;
	tps::Vec2i    m_Size;
	EObjectState  m_eObjectState;

}; /* !class GameObject */


} /* !namespace game */
