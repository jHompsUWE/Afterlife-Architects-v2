#include "pch.h"
#include "StructureTrainingCenter.h"

StructureTrainingCenter::StructureTrainingCenter(StructureData* structure_data, float _conversion_rate):
	StructureSprite(structure_data), conversion_rate(_conversion_rate)
{
	population_manager = GameplaySingletons::GetPopulationManager();
}

StructureTrainingCenter::~StructureTrainingCenter()
{
}

void StructureTrainingCenter::TickStructure(GameData* game_data)
{
	if ((population_manager->GetADPopulation(plane) + conversion_rate) <= population_manager->GetADCapacity(plane))
	{
		// Angels/Demons capacity is NOT full

		// Select a random soul type to convert
		ZoneType zone = ZoneType(rand() % 7 + 1);

		if (population_manager->GetZonePopulation(plane, zone) >= conversion_rate)
		{
			// There is enough soul to be converted
			population_manager->IncrementZonePopulation(plane, zone, -conversion_rate);
			population_manager->IncrementADPopulation(plane, conversion_rate);
		}
	}
}
