#pragma once

#include "GameData.h"
#include "StructureSprite.h"
#include "PopulationManager.h"
#include "VibeTilemap.h"
#include "RaDTilemap.h"
#include "ZoneType.h"

class StructureBuilding : public StructureSprite
{
public:
	StructureBuilding(ID3D11Device* GD, Vector2 width_height, Vector3 tile_pos, int _tile_size, ID3D11ShaderResourceView* texture,
		PlaneType _plane, ZoneType _zone, std::shared_ptr<PopulationManager> _population_manager, float _capacity);
	~StructureBuilding();

	void TickStructure(GameData* game_data) override;

protected:

private:
	bool CallEveryTicks(int tick_interval);
	void EvolveCheck();
	void EvolveStructure();
	void DevolveStructure();

	std::shared_ptr<PopulationManager> population_manager;

	ZoneType zone;

	float base_capacity; // How many souls this building can first hold
	float capacity; // How many souls this building can hold currently

	float tick_timer = 0; // Time between each evolve check
	int evolve_chance = 15; // Percentage chance for building to evolve after timer

	int level = 1; // Level of structure, linked with capacity and if it is 'demolished'
	int max_level = 3; // Maximum level a structure can reach

	int efficiency = 75;// IMPLEMENT LATER
};

