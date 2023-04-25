#include "pch.h"
#include "Sound.h"

// Checks volume 
// Checks the location of the sounds and what file type
// If wav it can be played. If not then it can't play
// Plays the file
Sound::Sound(AudioEngine* _audEngine, std::string _filename, float _volume)
{
	m_volume = _volume;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string fullpath = "../Sounds/" + _filename + ".wav";
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	m_sfx = std::make_unique<SoundEffect>(_audEngine, wFilename.c_str());
	if (m_sfx)
	{
		m_sfx->Play(m_volume, m_pitch, m_pan);
	}
}

Sound::~Sound()
{
	m_sfx.reset();
}

bool Sound::GetPlayState() const
{
	return m_sfx->IsInUse();
}
