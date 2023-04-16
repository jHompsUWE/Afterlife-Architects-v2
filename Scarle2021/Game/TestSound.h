#pragma once
#include "Sound.h"

//GEP:: An example sopund which goes off when you press space.

class TestSound :
	public Sound
{
public:
	TestSound(AudioEngine* _audEngine, string _filename, float _volume);
	~TestSound();

	virtual void Tick(GameData* _GD);
};

