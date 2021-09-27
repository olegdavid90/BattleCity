#pragma once

#include "Screen.hpp"
#include "system/types.hpp"
#include "renderer/Animator.hpp"

namespace game {


class PauseScreen : public Screen
{
  public:
	PauseScreen(class Core* pCore);

	void initText()     override;
	void updateInput()  override;
	void render() const override;

}; /* !class PauseScreen */


} /* !namespace game */
