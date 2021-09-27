#pragma once

#include "Screen.hpp"
#include "system/types.hpp"
#include "renderer/Animator.hpp"

namespace game {


class StartScreen : public Screen
{
  public:
	StartScreen(class Core* pCore);

	void initText()     override;
	void updateInput()  override;
	void render() const override;

  private:
	bool  m_isFirstLevel;

}; /* !class StartScreen */


} /* !namespace game */
