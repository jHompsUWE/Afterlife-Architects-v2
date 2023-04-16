#include "pch.h"
#include "StructureKarmaAnchor.h"

StructureKarmaAnchor::StructureKarmaAnchor(ID3D11Device* GD, Vector2 width_height, Vector3 tile_pos, int _tile_size, ID3D11ShaderResourceView* texture,
	PlaneType _plane, ZoneType _zone, std::shared_ptr<PopulationManager> _population_manager, float _reincarnated_souls) :
	StructureSprite(GD, width_height, tile_pos, _tile_size, texture, _plane), reincarnated_souls(_reincarnated_souls), population_manager(_population_manager), zone(_zone)
{
}

StructureKarmaAnchor::~StructureKarmaAnchor()
{
}

void StructureKarmaAnchor::TickStructure(GameData* game_data)
{
	// The souls will reincarnate after passing through the anchor
	// They will go back to the planet
	// The soul population will decrease depending on the number of souls that pass through the anchor
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