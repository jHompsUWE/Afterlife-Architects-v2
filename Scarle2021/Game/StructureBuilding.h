#pragma once

#include "GameData.h"
#include "StructureSprite.h"
#include "PopulationManager.h"
#include "EconomyManager.h"
#include "VibeTilemap.h"
#include "ZoneType.h"
#include "StructureData.h"
#include "GameplaySingletons.h"

class StructureBuilding : public StructureSprite
{
public:
	StructureBuilding(StructureData* structure_data, ZoneType _zone, std::unique_ptr<VibeTilemap>& _vibe_tilemap, float _capacity);
	~StructureBuilding();

	void TickStructure(GameData* game_data) override;

protected:

private:
	bool CallEveryTicks(int tick_interval);
	void EvolveCheck();
	void EvolveStructure();
	void DevolveStructure();
	int EfficiencyValue();

	std::shared_ptr<PopulationManager> population_manager;
	std::shared_ptr<EconomyManager> economy_manager;
	std::unique_ptr<VibeTilemap>& vibe_tilemap;

	ZoneType zone;
	Vector3 tile_pos;

	float base_capacity; // How many souls this building can first hold
	float capacity; // How many souls this building can hold currently

	float tick_timer = 0; // Time between each evolve check
	int evolve_chance = 15; // Percentage chance for building to evolve after timer

	int level = 1; // Level of structure, linked with capacity and if it is 'demolished'
	int max_level = 3; // Maximum level a structure can reach
};

