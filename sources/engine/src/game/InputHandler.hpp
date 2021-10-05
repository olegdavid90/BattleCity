#pragma once

#include "system/keys.hpp"

namespace game {


class InputHandler
{
  public:	
	static void update(bool (&keys)[3]);
	static void update(bool (&keys)[6]);

}; /* class InputHandler */


} /* !namespace game */
