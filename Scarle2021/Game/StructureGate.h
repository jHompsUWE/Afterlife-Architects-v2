#pragma once
#include <stdlib.h>

#include "PopulationManager.h"

#include "StructureSprite.h"
#include "ZoneType.h"

class StructureGate : public StructureSprite
{
public:
	StructureGate(ID3D11Device* GD, Vector2 width_height, Vector3 tile_pos, int _tile_size, ID3D11ShaderResourceView* texture, 
		PlaneType _plane, std::shared_ptr<PopulationManager> _population_manager, float _soul_rate);
	~StructureGate();

	void TickStructure(GameData* game_data) override;

protected:

private:
	std::shared_ptr<PopulationManager> population_manager;
	float soul_rate;
};

