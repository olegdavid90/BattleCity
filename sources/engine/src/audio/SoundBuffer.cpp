#include "SoundBuffer.hpp"

namespace game::audio {

//----------------------------------------------------------------------------//
SoundBuffer::SoundBuffer()
	: m_isLoaded{ false }
{ }

//----------------------------------------------------------------------------//
SoundBuffer::SoundBuffer(const std::string& path)
{
	this->m_isLoaded = sf::SoundBuffer::loadFromFile(path);
}

//----------------------------------------------------------------------------//
bool SoundBuffer::loaded() const
{
	return this->m_isLoaded;
}


} /* !namespace game::audio */
