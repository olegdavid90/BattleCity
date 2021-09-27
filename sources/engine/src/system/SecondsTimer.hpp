#pragma once

#include "FramesTimer.hpp"

namespace game {


class SecondsTimer : public FramesTimer
{
  public:
	using FramesTimer::FramesTimer;
	void update();

  private:
	unsigned  m_FramesCount{};

}; /* !class SecondsTimer */


} /* !namespace game */
