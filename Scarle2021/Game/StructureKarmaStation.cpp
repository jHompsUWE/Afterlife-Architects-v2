#include "pch.h"
#include "StructureKarmaStation.h"

StructureKarmaStation::StructureKarmaStation(ID3D11Device* GD, Vector2 width_height, Vector3 tile_pos, int _tile_size, ID3D11ShaderResourceView* texture,
	PlaneType _plane, ZoneType _zone, std::shared_ptr<PopulationManager> _population_manager, float _station_capacity) :
	StructureSprite(GD, width_height, tile_pos, _tile_size, texture, _plane), station_capacity(_station_capacity), population_manager(_population_manager), zone(_zone)
{
}


StructureKarmaStation::~StructureKarmaStation()
{
	// Decreases the number of souls in the station when the structure is destroyed
	population_manager->IncrementZoneCapacity(plane, zone, -station_capacity);
}

void StructureKarmaStation::TickStructure(GameData* game_data)
{
	// Function is called every few seconds
	// Generates a number of souls that will pass into the station from each zone
	ZoneType zone = ZoneType(rand() % 7 + 1);
	switch (plane)
	{
	case Heaven:
		population_manager->IncrementZonePopulation(Heaven, zone, station_capacity);
		break;

	case Hell:
		population_manager->IncrementZonePopulation(Hell, zone, station_capacity);
		break;
	}
}
