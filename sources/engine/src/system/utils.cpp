#include "utils.hpp"
#include "log.hpp"

#include <iterator>
#include <sstream>
#include <fstream>
#include <random>


//----------------------------------------------------------------------------//
void game::utils::stringToFile(const std::string& path, const std::string& data)
{
	std::ofstream ofstrm(path, std::ios_base::binary);
	if (ofstrm.is_open())
	{
		ofstrm << data;
		return;
	}

	LOG_ERROR("Failed to open file: '{}'", path);
}

//----------------------------------------------------------------------------//
std::string game::utils::fileToString(const std::string& path)
{
	std::ifstream ifstrm(path, std::ios_base::binary);
	if (ifstrm.is_open())
	{
		std::noskipws(ifstrm);
		return { std::istream_iterator<char>{ ifstrm }, {} };
	}

	LOG_ERROR("Failed to open file: '{}'", path);
	return {};
}

//----------------------------------------------------------------------------//
std::uint8_t game::utils::rand()
{
	static std::mt19937 generator{ std::random_device{}() };

	return generator();
}
