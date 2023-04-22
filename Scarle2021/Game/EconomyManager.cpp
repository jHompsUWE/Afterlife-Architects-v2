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

/// <summary>
/// Function called every in-game year
/// </summary>
/// <param name="dt">Delta time of frame</param>
/// <param name="time_interval">Duration of in-game year</param>
/// <returns>Boolean if an in-game year has passed</returns>
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

/// <summary>
/// Reduce currency based on structure cost given
/// </summary>
/// <param name="cost">Structure cost</param>
/// <returns>Boolean on if structure is bought</returns>
bool EconomyManager::PurchaseStructure(int cost)
{
    if (money >= cost)
    {
        money -= cost;
        return true;
    }
    return false;
}

/// <summary>
/// All money generation and loss calculations called every in-game year
/// </summary>
void EconomyManager::YearUpdate()
{
    /// Advisor event for no roads being placed down
    AL::NewEventManager::GenerateEventSt(AL::event_adv_fault, "Road", total_roads == 0);

    year++;

    // End function if no souls to calculate economy with
    if (pop_manager->GetTotalCapacity() == 0)
    {
        return;
    }

    // Road upkeep cost
    money -= total_roads * cost_per_road;

    // Calculate averate tile population and population gain
    float total_capacity = pop_manager->GetTotalCapacity();
    float average_tile_population = total_capacity / total_structures;
    float previous_soul_gain = total_capacity - prev_year_population;
    prev_year_population = total_capacity;

    // Soul gain for new souls entered
    float soul_rate = average_tile_population / year;
    money += previous_soul_gain * soul_rate;

    // Soul gain for total population in buildings
    money += money_per_soul * total_capacity * (rad_prod_percent / 50);
}