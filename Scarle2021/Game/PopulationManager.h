#pragma once
#include "ZoneType.h"
#include "PlaneType.h"
#include <iostream>
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
	int GetTotalSoulsForPlane(PlaneType plane);
	int GetTotalCapacity();

	int GetADPopulation(PlaneType plane);
	int GetADCapacity(PlaneType plane);

	bool GetZoneFull(PlaneType plane, ZoneType zone);

	bool PopulationCheck(PlaneType plane, ZoneType zone);

	void IncrementReligions(PlaneType plane, int pop_inc);
	int GetReligiousSpread(PlaneType plane, int rel);

protected:

private:
	// Heaven
	std::vector<int> heaven_population;
	std::vector<int> heaven_capacity;
	std::vector<bool> heaven_full;
	int population_angel;
	int capacity_angel;

	// Religion Heaven
	// HAHA, HOHO, OPRA, OCRA, ALF, RALF, SUMA, SUSA
	int heaven_religion_spread[8] = { 0,0,0,0,0,0,0,0 };


	// Hell
	std::vector<int> hell_population;
	std::vector<int> hell_capacity;
	std::vector<bool> hell_full;
	int population_demon;
	int capacity_demon;

	// Religion Hell
	// HAHA, HOHO, OPRA, OCRA, ALF, RALF, SUMA, SUSA
	int hell_religion_spread[8] = { 0,0,0,0,0,0,0,0 };
};

