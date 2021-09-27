#include "SecondsTimer.hpp"


void game::SecondsTimer::update()
{
	if (this->m_isRunning)
	{
		++this->m_FramesCount;

		if ((this->m_FramesCount & 0X3F) == 0)
		{
			--this->m_TimeLeft;
		}

		if (this->m_TimeLeft == 0)
		{
			this->m_isRunning = false;
			this->m_Callback();
		}
	}
}
