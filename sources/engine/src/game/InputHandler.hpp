#pragma once

#include "system/keys.hpp"

namespace game {


class InputHandler
{
  public:	
	void update(bool (&keys)[3]);
	void update(bool (&keys)[6]);

}; /* class InputHandler */


} /* !namespace game */
