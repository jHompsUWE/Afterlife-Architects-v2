#pragma once
#include <memory>

#include "PopulationManager.h"
#include "StructureSprite.h"

#include "StructureData.h"
#include "GameplaySingletons.h"

class StructureTopia : public StructureSprite
{
public:
	StructureTopia(StructureData* structure_data, float _capacity);
	~StructureTopia();

	void TickStructure(GameData* game_data) override;

protected:

private:
	float capacity; // How many angels/demons this building can hold
	std::shared_ptr<PopulationManager> population_manager;
};

