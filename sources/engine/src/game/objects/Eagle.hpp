#pragma once

#include "StaticObject.hpp"
#include "system/types.hpp"

#include <array>

namespace game {

namespace renderer { class Sprite2D; }


class Eagle : public StaticObject
{
  public:
	Eagle(class Level* parent_level);

	void render() const                     override;
	void onCollision(DynamicObject* object) override;


  private:
	renderer::Sprite2D*  m_pSprite;
	class Level*         m_pParentLevel;

}; /* !class Eagle */


} /* !namespace game */
