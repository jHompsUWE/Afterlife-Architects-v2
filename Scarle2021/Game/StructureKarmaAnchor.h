#pragma once

#include "PopulationManager.h"

#include "StructureSprite.h"
#include "ZoneType.h"

class StructureKarmaAnchor : public StructureSprite
{
public:
	StructureKarmaAnchor(ID3D11Device* GD, Vector2 width_height, Vector3 tile_pos, int _tile_size, ID3D11ShaderResourceView* texture,
		PlaneType _plane, std::shared_ptr<PopulationManager> _population_manager, float _number_of_souls);
	~StructureKarmaAnchor();

	void TickStructure(GameData* game_data) override;

protected:

private:
	std::shared_ptr<PopulationManager> population_manager;
	float number_of_souls;
};