#include "pch.h"
#include "StructureKarmaAnchor.h"

StructureKarmaAnchor::StructureKarmaAnchor(ID3D11Device* GD, Vector2 width_height, Vector3 tile_pos, int _tile_size, ID3D11ShaderResourceView* texture,
	PlaneType _plane, ZoneType _zone, std::shared_ptr<PopulationManager> _population_manager, float _reincarnated_souls) :
	StructureSprite(GD, width_height, tile_pos, _tile_size, texture, _plane), reincarnated_souls(_reincarnated_souls), population_manager(_population_manager), zone(_zone)
{
	population_manager->IncrementZonePopulation(plane, zone, reincarnated_souls);
}

StructureKarmaAnchor::~StructureKarmaAnchor()
{
	population_manager->IncrementZonePopulation(plane, zone, -reincarnated_souls);
}

void StructureKarmaAnchor::TickStructure(GameData* game_data)
{
	// Insert Functionality for Karma Anchor here
	// This function is called every few seconds instead of per frame
	// Reincarnate souls back to the planet
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
