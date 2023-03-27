#include "pch.h"
#include "StructureKarmaStation.h"

StructureKarmaStation::StructureKarmaStation(ID3D11Device* GD, Vector2 width_height, Vector3 tile_pos, int _tile_size, ID3D11ShaderResourceView* texture,
	PlaneType _plane, ZoneType _zone, std::shared_ptr<PopulationManager> _population_manager, float _station_capacity) :
	StructureSprite(GD, width_height, tile_pos, _tile_size, texture, _plane), station_capacity(_station_capacity), population_manager(_population_manager), zone(_zone)
{
	population_manager->IncrementZoneCapacity(plane,zone, station_capacity);
}


StructureKarmaStation::~StructureKarmaStation()
{
	// Decrements souls in the zones when the building is destroyed
	population_manager->IncrementZoneCapacity(plane, zone, -station_capacity);
}

void StructureKarmaStation::TickStructure(GameData* game_data)
{
	// Insert Functionality for Karma Station here
	// This function is called every few seconds instead of per frame
}
