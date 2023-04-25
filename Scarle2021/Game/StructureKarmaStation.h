#pragma once
#include <stdlib.h>

#include "PopulationManager.h"

#include "StructureSprite.h"
#include "ZoneType.h"

#include "StructureData.h"
#include "GameplaySingletons.h"

class StructureKarmaStation : public StructureSprite
{
public:
	StructureKarmaStation(StructureData* structure_data, float _station_souls);
	~StructureKarmaStation();

	void TickStructure(GameData* game_data) override;

protected:

private:
	std::shared_ptr<PopulationManager> population_manager;
	float station_souls;
};

