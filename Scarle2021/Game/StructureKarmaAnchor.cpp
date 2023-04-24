#include "pch.h"
#include "StructureKarmaAnchor.h"

StructureKarmaAnchor::StructureKarmaAnchor(StructureData* structure_data, float _reincarnated_souls) :
	StructureSprite(structure_data), reincarnated_souls(_reincarnated_souls)
{
	population_manager = GameplaySingletons::GetPopulationManager();
}

StructureKarmaAnchor::~StructureKarmaAnchor()
{
}

void StructureKarmaAnchor::TickStructure(GameData* game_data)
{
	// Takes away a number of souls in the zones that have been reincarnated
	// Generate souls based on the soul_rate for a random zone in the plane this gate is in
	ZoneType zone = ZoneType(rand() % 7 + 1);
	switch (plane)
	{
	case Heaven:
		population_manager->IncrementZonePopulation(Heaven, zone, -reincarnated_souls);
		break;

	case Hell:
		population_manager->IncrementZonePopulation(Hell, zone, -reincarnated_souls);
		break;
	}
}
