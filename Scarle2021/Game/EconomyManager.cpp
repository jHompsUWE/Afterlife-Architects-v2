#include "pch.h"
#include "EconomyManager.h"

EconomyManager::EconomyManager(std::shared_ptr<PopulationManager> _pop_manager) :
    pop_manager(_pop_manager)
{
	money = 333333;
	year = 0;
    total_roads = 0;
    total_karma_tracks = 0;
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
    money -= total_karma_tracks * cost_per_karma_track;

    if (pop_manager->GetTotalCapacity() == 0)
    {
        return;
    }
    int total_structures = 100;
    float average_tile_population = pop_manager->GetTotalCapacity() / total_structures;

    float previous_soul_gain = pop_manager->GetTotalCapacity() - prev_year_population;
    prev_year_population = pop_manager->GetTotalCapacity();

    // Soul gain for new souls entered
    float soul_rate = average_tile_population / year;
    money += previous_soul_gain * soul_rate;

    // Soul gain for total population in buildings
    money += money_per_soul * pop_manager->GetTotalCapacity() * (rad_prod_percent / 50);
}