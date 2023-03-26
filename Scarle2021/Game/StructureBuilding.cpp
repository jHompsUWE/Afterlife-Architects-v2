#include "pch.h"
#include "StructureBuilding.h"

StructureBuilding::StructureBuilding(ID3D11Device* GD, Vector2 width_height, Vector3 _tile_pos, Vector3 _start, int _tile_size, ID3D11ShaderResourceView* texture, 
	PlaneType _plane, ZoneType _zone, std::shared_ptr<PopulationManager> _population_manager, std::unique_ptr<VibeTilemap>& _vibe_tilemap, std::unique_ptr<RaDTilemap>& _rad_tilemap, float _capacity):
	StructureSprite(GD, width_height, _tile_pos, _tile_size, texture, _plane), base_capacity(_capacity), capacity(_capacity), vibe_tilemap(_vibe_tilemap), rad_tilemap(_rad_tilemap), population_manager(_population_manager), zone(_zone), tile_pos(_tile_pos), start(_start)
{
	// Increment soul capacity on creation
	srand(time(0));
	population_manager->IncrementZoneCapacity(plane, zone, capacity);
}

StructureBuilding::~StructureBuilding()
{
	// Decrement soul capacity on creation
	population_manager->IncrementZoneCapacity(plane, zone, -capacity);
}

void StructureBuilding::TickStructure(GameData* game_data)
{
	if (CallEveryTicks(4) && (rand() % 100) < evolve_chance)
	{
		EvolveCheck();
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
		std::cout << "Structure evolved" << std::endl;
		level += 1;
		population_manager->IncrementZoneCapacity(plane, zone, capacity);
	}
}

void StructureBuilding::DevolveStructure()
{
	if (level > 0)
	{
		std::cout << "Structure devolved" << std::endl;
		level -= 1;
		population_manager->IncrementZoneCapacity(plane, zone, -base_capacity);
	}
}

int StructureBuilding::EfficiencyValue()
{
	int eff_val = (vibe_tilemap->GetVibe(tile_pos - start) * (100/32) + 50) + (rad_tilemap->GetRaD(tile_pos-start));
	eff_val = eff_val * 0.5f;
	std::cout << eff_val << std::endl;
	return eff_val;
}