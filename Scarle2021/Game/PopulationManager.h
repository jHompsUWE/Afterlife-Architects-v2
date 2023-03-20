#pragma once
#include "ZoneType.h"
#include "PlaneType.h"
#include <vector>

class PopulationManager
{
public:
	PopulationManager();
	~PopulationManager();

	void IncrementZonePopulation(PlaneType plane, ZoneType zone, int population_increase);
	void IncrementZoneCapacity(PlaneType plane, ZoneType zone, int capacity_increase);

	void IncrementADPopulation(PlaneType plane, int population_increase);
	void IncrementADCapacity(PlaneType plane, int capacity_increase);

	void SetZoneFull(PlaneType plane, ZoneType zone, bool is_full);

	int GetZonePopulation(PlaneType plane, ZoneType zone);
	int GetZoneCapacity(PlaneType plane, ZoneType zone);
	int GetTotalSouls();

	int GetADPopulation(PlaneType plane);
	int GetADCapacity(PlaneType plane);

	bool GetZoneFull(PlaneType plane, ZoneType zone);

	bool PopulationCheck(PlaneType plane, ZoneType zone);

protected:

private:
	// Heaven
	std::vector<int> heaven_population;
	std::vector<int> heaven_capacity;
	std::vector<bool> heaven_full;
	int population_angel;
	int capacity_angel;


	// Hell
	std::vector<int> hell_population;
	std::vector<int> hell_capacity;
	std::vector<bool> hell_full;
	int population_demon;
	int capacity_demon;
};

