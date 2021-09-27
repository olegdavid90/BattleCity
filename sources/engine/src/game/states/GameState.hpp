#pragma once

namespace game {


class GameState
{
  public:
	GameState(class Core* pCore);
    virtual ~GameState() = default;
	
	virtual void updateInput()  = 0;
	virtual void update()       = 0;
	virtual void render() const = 0;

  protected:
	virtual void initText()     = 0;


  protected:
	class Core*  m_pCore;
	
}; /* !class GameState */


inline GameState::GameState(Core* pCore)
	: m_pCore{ pCore }
{ }


} /* !namespace game */
