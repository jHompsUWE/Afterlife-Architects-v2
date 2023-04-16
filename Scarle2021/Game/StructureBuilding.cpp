#include "pch.h"
#include "StructureBuilding.h"

StructureBuilding::StructureBuilding(StructureData* structure_data, ZoneType _zone, std::unique_ptr<VibeTilemap>& _vibe_tilemap, std::unique_ptr<RaDTilemap>& _rad_tilemap, float _capacity) :
	StructureSprite(structure_data), base_capacity(_capacity), capacity(_capacity), vibe_tilemap(_vibe_tilemap), rad_tilemap(_rad_tilemap), zone(_zone), tile_pos(structure_data->tile_pos)
{
	// Increment soul capacity on creation
	srand(time(0));
	population_manager = GameplaySingletons::GetPopulationManager();
	economy_manager = GameplaySingletons::GetEconomyManager();
	population_manager->IncrementZoneCapacity(plane, zone, capacity);
}

StructureBuilding::~StructureBuilding()
{
	// Decrement soul capacity on creation
	population_manager->IncrementZoneCapacity(plane, zone, -capacity);
}

void StructureBuilding::TickStructure(GameData* game_data)
{
	if (CallEveryTicks(4))
	{
		if (rand() % 100 < evolve_chance)
		{
			EvolveCheck();
		}
	}
}

/// <summary>
/// A timer that can be used to call a function at every given time interval
/// </summary>
/// <param name="dt">Delta time</param>
/// <param name="time_interval">Time interval per call</param>
/// <returns>True if time interval has passed, False if not</returns>
bool StructureBuilding::CallEveryTicks(int tick_interval)
{
	tick_timer += 1;
	if (tick_timer > tick_interval)
	{
		tick_timer = 0;
		return true;
	}
	return false;
}

void StructureBuilding::EvolveCheck()
{
	if (EfficiencyValue() <= 25)
	{
		DevolveStructure();
	}
	else if (EfficiencyValue() >= 75)
	{
		EvolveStructure();
	}
}

void StructureBuilding::EvolveStructure()
{
	if (level < max_level)
	{
		//std::cout << "Structure evolved" << std::endl;
		level += 1;
		population_manager->IncrementZoneCapacity(plane, zone, capacity);
	}
}

void StructureBuilding::DevolveStructure()
{
	if (level > 0)
	{
		//std::cout << "Structure devolved" << std::endl;
		level -= 1;
		population_manager->IncrementZoneCapacity(plane, zone, -base_capacity);
	}
}

int StructureBuilding::EfficiencyValue()
{
	//int eff_val = (vibe_tilemap->GetVibe(tile_pos - start) * (100/32) + 50) + (rad_tilemap->GetRaD(tile_pos-start));
	int eff_val = (vibe_tilemap->GetVibe(tile_pos) * (100/32) + 50) + (100 - economy_manager->GetRaD());
	eff_val = eff_val * 0.5f;
	//std::cout << eff_val << std::endl;
	return eff_val;
}