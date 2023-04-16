#pragma once
#include <stdlib.h>

#include "PopulationManager.h"

#include "StructureSprite.h"
#include "ZoneType.h"

#include "StructureData.h"
#include "GameplaySingletons.h"

class StructureGate : public StructureSprite
{
public:
	StructureGate(StructureData* structure_data, float _soul_rate);
	~StructureGate();

	void TickStructure(GameData* game_data) override;

protected:

private:
	std::shared_ptr<PopulationManager> population_manager;
	float soul_rate;
};

