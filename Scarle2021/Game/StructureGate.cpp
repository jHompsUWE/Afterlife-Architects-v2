#include "pch.h"
#include "StructureGate.h"

StructureGate::StructureGate(ID3D11Device* GD, Vector2 width_height, Vector3 tile_pos, int _tile_size, ID3D11ShaderResourceView* texture, 
	PlaneType _plane, std::shared_ptr<PopulationManager> _population_manager, float _soul_rate) :
	StructureSprite(GD, width_height, tile_pos, _tile_size, texture, _plane), soul_rate(_soul_rate), population_manager(_population_manager)
{
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
