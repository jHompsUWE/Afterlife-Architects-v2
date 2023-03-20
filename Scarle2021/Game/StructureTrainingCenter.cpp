#include "pch.h"
#include "StructureTrainingCenter.h"

StructureTrainingCenter::StructureTrainingCenter(ID3D11Device* GD, Vector2 width_height, Vector3 tile_pos, int _tile_size, 
	ID3D11ShaderResourceView* texture, PlaneType _plane, std::shared_ptr<PopulationManager> _population_manager, float _conversion_rate):
	StructureSprite(GD, width_height, tile_pos, _tile_size, texture, _plane), population_manager(_population_manager), conversion_rate(_conversion_rate)
{
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
