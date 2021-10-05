#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

namespace game  {
namespace audio {


class SoundBuffer : public sf::SoundBuffer
{
  public:
	SoundBuffer();
	SoundBuffer(const std::string& path);

	bool loaded() const;
	
	
  private:
	bool  m_isLoaded;

}; /* !class SoundBuffer */


} /* !namespace audio */
} /* !namespace game */
