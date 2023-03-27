#include "pch.h"
#include "StructureTopia.h"

StructureTopia::StructureTopia(StructureData* structure_data, float _capacity):
	StructureSprite(structure_data), capacity(_capacity)
{
	population_manager = GameplaySingletons::GetPopulationManager();

	// Increment Angels capacity on creation
	population_manager->IncrementADCapacity(plane, capacity);
}

StructureTopia::~StructureTopia()
{
	// Decrement Angels capacity on destruction
	population_manager->IncrementADCapacity(plane, -capacity);
}

void StructureTopia::TickStructure(GameData* game_data)
{
}

