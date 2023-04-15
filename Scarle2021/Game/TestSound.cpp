#include "pch.h"
#include "TestSound.h"
#include "GameData.h"


TestSound::TestSound(AudioEngine* _audEngine, string _filename, float _volume) :Sound(_audEngine, _filename, _volume)
{
}


TestSound::~TestSound()
{
}

void TestSound::Tick(GameData * _GD)
{
	// if (_GD->keyboard_state_tracker.pressed.Space)
	// {
	// 	  Play();
	// }
}
