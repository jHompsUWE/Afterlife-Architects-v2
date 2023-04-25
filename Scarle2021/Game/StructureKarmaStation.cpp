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
	// This function runs every few seconds rather than every frame
	// Increase the number of souls in this zone when the structure is created
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
