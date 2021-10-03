
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
		Disabled
	};

	GameObject(const tps::Vec2i& position, const tps::Vec2i& size);
	virtual ~GameObject() = default;

	virtual void render() const                    = 0;
	virtual void onCollision(class DynamicObject*) = 0;

	virtual void update();
	virtual void setPosition(const tps::Vec2i& position);
	virtual bool isDisabled() const;

	void setPosition(int x, int y);
	void setSize(tps::Vec2i size);
	void setSize(int width, int height);
	const tps::Vec2i& getSize()     const;
	const tps::Vec2i& getPosition() const;
	tps::IntRect getGlobalBounds()  const;


  protected:
	tps::Vec2i    m_Position;
	tps::Vec2i    m_Size;
	EObjectState  m_eObjectState;

}; /* !class GameObject */


} /* !namespace game */
