#include "pch.h"
#include "StructureKarmaStation.h"

StructureKarmaStation::StructureKarmaStation(StructureData* structure_data, float _station_souls) :
	StructureSprite(structure_data), station_souls(_station_souls)
{
	population_manager = GameplaySingletons::GetPopulationManager();
}

StructureKarmaStation::~StructureKarmaStation()
{
}

void StructureKarmaStation::TickStructure(GameData* game_data)
{
	// Generate souls based on the soul_rate for a random zone in the plane this gate is in
	ZoneType zone = ZoneType(rand() % 7 + 1);
	switch (plane)
	{
	case Heaven:
		population_manager->IncrementZoneCapacity(Heaven, zone, station_souls);
		break;

	case Hell:
		population_manager->IncrementZoneCapacity(Hell, zone, station_souls);
		break;
	}
}
