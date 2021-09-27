#pragma once

namespace game {
	
struct NonCopyable
{
	NonCopyable()                              = default;
	NonCopyable(const NonCopyable&)            = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;
	
}; /* !struct NonCopyable */


} /* !namespace game */
