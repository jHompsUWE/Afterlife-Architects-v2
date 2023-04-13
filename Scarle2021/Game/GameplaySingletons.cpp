#include "pch.h"
#include "GameplaySingletons.h"

GameplaySingletons& GameplaySingletons::Get()
{
	static GameplaySingletons instance;
	return instance;
}

GameplaySingletons::GameplaySingletons()
{
	population_manager = std::make_shared<PopulationManager>();
	economy_manager = std::make_shared<EconomyManager>(population_manager);

	std::ifstream file("../Game/JSON_Files/StructureValues.json");
	structure_values = nlohmann::json::parse(file);
}
