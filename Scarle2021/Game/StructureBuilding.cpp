#include "pch.h"
#include "StructureBuilding.h"

StructureBuilding::StructureBuilding(StructureData* structure_data, ZoneType _zone, float _capacity):
	StructureSprite(structure_data), capacity(_capacity), zone(_zone)
{
	// Increment soul capacity on creation
	population_manager = GameplaySingletons::GetPopulationManager();
	population_manager->IncrementZoneCapacity(plane, zone, capacity);
}

StructureBuilding::~StructureBuilding()
{
	// Decrement soul capacity on creation
	population_manager->IncrementZoneCapacity(plane, zone, -capacity);
}

void StructureBuilding::TickStructure(GameData* game_data)
{
}
