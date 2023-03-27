#include "pch.h"
#include "EconomyManager.h"

EconomyManager::EconomyManager(std::shared_ptr<PopulationManager> _pop_manager) :
    pop_manager(_pop_manager)
{
	money = 333333;
	year = 0;
    total_roads = 0;
    timer = 0;
    prev_year_population = 0;
}

void EconomyManager::Tick(GameData* game_data)
{
    if (CallEverySeconds(game_data->delta_time, time_per_year))
    {
        YearUpdate();
    }
}

bool EconomyManager::CallEverySeconds(float dt, float time_interval)
{
    timer += dt;
    if (timer >= time_interval)
    {
        timer -= time_interval;
        return true;
    }
    return false;
}

bool EconomyManager::PurchaseStructure(int cost)
{
    if (money >= cost)
    {
        money -= cost;
        return true;
    }
    return false;
}

void EconomyManager::YearUpdate()
{
    year++;
    money -= total_roads * cost_per_road;

    // CHANGE TO FUNCTION RELATED WITH POPULATION MANAGER AND BUILDING MANAGER
    if (pop_manager->GetTotalSouls() == 0)
    {
        return;
    }
    int total_structures = 100;
    float average_tile_population = pop_manager->GetTotalSouls() / total_structures;

    float previous_soul_gain = pop_manager->GetTotalSouls() - prev_year_population;
    prev_year_population = pop_manager->GetTotalSouls();

    float soul_rate = average_tile_population / year;
    money += previous_soul_gain * soul_rate;
}