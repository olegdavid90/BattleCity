#pragma once

#include "Screen.hpp"
#include "system/types.hpp"
#include "renderer/Animator.hpp"

namespace game {


class GameOverScreen : public Screen
{
  public:
	GameOverScreen(class Core* pCore);

	void initText()     override;
	void updateInput()  override;
	void render() const override;

}; /* !class GameOverScreen */


} /* !namespace game */
