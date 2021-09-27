#pragma once

#include "game/Core.hpp"
#include "GameState.hpp"
#include "system/types.hpp"
#include "system/FramesTimer.hpp"
#include "renderer/Animator.hpp"

namespace game {

namespace renderer { 
	class Sprite2D;
	class Text2D;
} /* !namespace renderer */


class Screen : public GameState
{
  public:
	Screen(class Core* pCore, EGameState screen_type);
	virtual ~Screen() = 0;

	void update()       override;
	void initText()     override;
	void updateInput()  override;
	void render() const override;


  protected:							          
	tps::Vec2i                               m_TankPosition;									         
	std::unique_ptr<class Title>             m_Title;
	std::unique_ptr<class renderer::Text2D>  m_MenuText;
	std::unique_ptr<class renderer::Text2D>  m_ScoreText;
	renderer::Sprite2D*                      m_pTankSprite;
	renderer::Animator                       m_TankAnimator;
	FramesTimer                              m_ShowTimer;

	int                                      m_maxMenuSelection;
	int                                      m_MenuSelection;
	bool                                     m_Keys[3];	
	bool                                     m_isShowTime;

}; /* !class Screen */


} /* !namespace game */
