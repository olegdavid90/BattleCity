#pragma once

#include <functional>

namespace game {


class FramesTimer
{
  public:
	FramesTimer();

	void update();
	void start(unsigned duration);
	void setCallback(const std::function<void()>& callback);


  protected:
	std::function<void()> m_Callback;
	unsigned              m_TimeLeft;
	bool                  m_isRunning;

}; /* !class SecondTimer */


} /* !namespace game */
