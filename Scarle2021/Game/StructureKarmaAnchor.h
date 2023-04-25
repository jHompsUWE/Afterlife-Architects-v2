#pragma once
#include <stdlib.h>

#include "PopulationManager.h"

#include "StructureSprite.h"
#include "ZoneType.h"

#include "StructureData.h"
#include "GameplaySingletons.h"

class StructureKarmaAnchor : public StructureSprite
{
public:
	StructureKarmaAnchor(StructureData* structure_data, float _reincarnated_souls);
	~StructureKarmaAnchor();

	void TickStructure(GameData* game_data) override;

protected:

private:
	std::shared_ptr<PopulationManager> population_manager;
	float reincarnated_souls;
};

