#include "pch.h"
#include "StructureKarmaAnchor.h"

StructureKarmaAnchor::StructureKarmaAnchor(ID3D11Device* GD, Vector2 width_height, Vector3 tile_pos, int _tile_size, ID3D11ShaderResourceView* texture,
	PlaneType _plane, std::shared_ptr<PopulationManager> _population_manager, float _number_of_souls) :
	StructureSprite(GD, width_height, tile_pos, _tile_size, texture, _plane), number_of_souls(_number_of_souls), population_manager(_population_manager)
{
}

StructureKarmaAnchor::~StructureKarmaAnchor()
{
}

void StructureKarmaAnchor::TickStructure(GameData* game_data)
{
	// Insert Functionality for Karma Anchor here
	// This function is called every few seconds instead of per frame
}
