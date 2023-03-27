#include "pch.h"
#include "StructureGate.h"

StructureGate::StructureGate(StructureData* structure_data, float _soul_rate) :
	StructureSprite(structure_data), soul_rate(_soul_rate)
{
	population_manager = GameplaySingletons::GetPopulationManager();
}

StructureGate::~StructureGate()
{
}

void StructureGate::TickStructure(GameData* game_data)
{
	// Generate souls based on the soul_rate for a random zone in the plane this gate is in
	ZoneType zone = ZoneType(rand() % 7 + 1);
	switch (plane)
	{
	case Heaven:
		population_manager->IncrementZonePopulation(Heaven, zone, soul_rate);
		break;

	case Hell:
		population_manager->IncrementZonePopulation(Hell, zone, soul_rate);
		break;
	}
}
