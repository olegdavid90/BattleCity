#pragma once

#include <cstdint>
#include <string>

namespace game {
namespace utils {
	

void stringToFile(const std::string& path, const std::string& data);
std::string fileToString(const std::string& path);
std::uint8_t rand();


} /* !namespace utils */	
} /* !namespace game */
