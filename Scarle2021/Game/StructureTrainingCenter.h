#pragma once

#include "StructureSprite.h"
#include "PopulationManager.h"
#include "ZoneType.h"

#include "StructureData.h"
#include "GameplaySingletons.h"

class StructureTrainingCenter: public StructureSprite
{
public:
	StructureTrainingCenter(StructureData* structure_data, float _conversion_rate);
	~StructureTrainingCenter();

	void TickStructure(GameData* game_data) override;

protected:

private:
	std::shared_ptr<PopulationManager> population_manager;
	float conversion_rate; // Conversion of souls to angels/demons
};

