#include "pch.h"
#include "EconomyManager.h"

EconomyManager::EconomyManager()
{
	money = 999999;
	year = 0;
    timer = 0;
}

void EconomyManager::Tick(GameData* game_data)
{
    if (CallEverySeconds(game_data->delta_time, 5))
    {
        year++;
    }

	// TODO:: an actual economy system
	money += game_data->delta_time * 10000;
}

bool EconomyManager::CallEverySeconds(float dt, float time_interval)
{
    timer += dt;
    if (timer > time_interval)
    {
        timer = 0;
        return true;
    }
    return false;
}
