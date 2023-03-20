#pragma once
#include <memory>
#include "PopulationManager.h"
#include "EconomyManager.h"

class GameplaySingletons
{
public:
	GameplaySingletons(const GameplaySingletons&) = delete;

	GameplaySingletons& operator=(const GameplaySingletons&) = delete;

	static std::shared_ptr<PopulationManager> GetPopulationManager() { return Get().population_manager; }
	static std::shared_ptr<EconomyManager> GetEconomyManager() { return Get().economy_manager; }

protected:

private:
	GameplaySingletons();
	~GameplaySingletons() = default;

	static GameplaySingletons& Get();

	std::shared_ptr<PopulationManager> population_manager;
	std::shared_ptr<EconomyManager> economy_manager;
};

