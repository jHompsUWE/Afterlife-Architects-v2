#pragma once

#include "PopulationManager.h"

#include "StructureSprite.h"
#include "ZoneType.h"
#include "Tilemap.h"
#include "Tile.h"

class StructureKarmaStation : public StructureSprite
{
public:
	StructureKarmaStation(ID3D11Device* GD, Vector2 width_height, Vector3 tile_pos, int _tile_size, ID3D11ShaderResourceView* texture,
		PlaneType _plane, ZoneType _zone, std::shared_ptr<PopulationManager> _population_manager, float _station_capacity);
	~StructureKarmaStation();

	void TickStructure(GameData* game_data) override;
	

protected:

private:
	std::shared_ptr<PopulationManager> population_manager;
	float station_capacity;
	ZoneType zone;

};

