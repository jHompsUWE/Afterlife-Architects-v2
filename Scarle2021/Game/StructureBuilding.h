#pragma once

#include "StructureSprite.h"
#include "PopulationManager.h"
#include "ZoneType.h"
#include "StructureData.h"
#include "GameplaySingletons.h"

class StructureBuilding : public StructureSprite
{
public:
	StructureBuilding(StructureData* structure_data, ZoneType _zone, float _capacity);
	~StructureBuilding();

	void TickStructure(GameData* game_data) override;

protected:

private:
	float capacity; // How many souls this building can hold
	std::shared_ptr<PopulationManager> population_manager;
	ZoneType zone;
};

