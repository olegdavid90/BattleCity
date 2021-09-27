#include "FramesTimer.hpp"


game::FramesTimer::FramesTimer()
	: m_TimeLeft{}
	, m_isRunning{}
{ }


void game::FramesTimer::update()
{
	if (this->m_isRunning)
	{
		--this->m_TimeLeft;

		if (this->m_TimeLeft == 0)
		{
			this->m_isRunning = false;
			this->m_Callback();
		}
	}
}


void game::FramesTimer::start(unsigned duration)
{
	this->m_TimeLeft  = duration;
	this->m_isRunning = true;
}


void game::FramesTimer::setCallback(const std::function<void()>& callback)
{
	this->m_Callback = callback;
}
