#include "pch.h"
#include "PopulationManager.h"

PopulationManager::PopulationManager()
{
	// Heaven
	heaven_population = std::vector<int>(7, 0);
	heaven_capacity = std::vector<int>(7, 0);
	heaven_full = std::vector<bool>(7, true);
	capacity_angel = 0;

	// Hell
	hell_population = std::vector<int>(7, 0);
	hell_capacity = std::vector<int>(7, 0);
	hell_full = std::vector<bool>(7, true);
	capacity_demon = 0;
}

PopulationManager::~PopulationManager()
{
}

void PopulationManager::IncrementZonePopulation(PlaneType plane, ZoneType zone, int population_increase)
{		
	switch (plane)
	{
	case Heaven:
		heaven_population[(int)zone - 1] += population_increase;
		break;

	case Hell:
		hell_population[(int)zone - 1] += population_increase;
		break;
	}
	IncrementReligions(plane, population_increase);
}

void PopulationManager::IncrementZoneCapacity(PlaneType plane, ZoneType zone, int capacity_increase)
{
	switch (plane)
	{
	case Heaven:
		heaven_capacity[(int)zone - 1] += capacity_increase;
		break;

	case Hell:
		hell_capacity[(int)zone - 1] += capacity_increase;
		break;
	}
}

void PopulationManager::IncrementADPopulation(PlaneType plane, int population_increase)
{
	switch (plane)
	{
	case Heaven:
		population_angel += population_increase;
		break;

	case Hell:
		population_demon += population_increase;
		break;
	}
}

void PopulationManager::IncrementADCapacity(PlaneType plane, int capacity_increase)
{
	switch (plane)
	{
	case Heaven:
		capacity_angel += capacity_increase;
		break;

	case Hell:
		capacity_demon += capacity_increase;
		break;
	}
}

void PopulationManager::SetZoneFull(PlaneType plane, ZoneType zone, bool is_full)
{
	switch (plane)
	{
	case Heaven:
		heaven_full[(int)zone - 1] = is_full;
		break;

	case Hell:
		hell_full[(int)zone - 1] = is_full;
		break;
	}
}

int PopulationManager::GetZonePopulation(PlaneType plane, ZoneType zone)
{
	switch (plane)
	{
	case Heaven:
		return heaven_population[(int)zone - 1];

	case Hell:
		return hell_population[(int)zone - 1];
	}

	return 0;
}

int PopulationManager::GetZoneCapacity(PlaneType plane, ZoneType zone)
{
	switch (plane)
	{
	case Heaven:
		return heaven_capacity[(int)zone - 1];

	case Hell:
		return hell_capacity[(int)zone - 1];
	}

	return 0;
}

int PopulationManager::GetTotalSouls()
{
	int total = 0;

	for (auto& soul: heaven_population)
	{
		total += soul;
	}

	for (auto& soul : hell_population)
	{
		total += soul;
	}

	return total;
}

int PopulationManager::GetTotalCapacity()
{
	int total = 0;

	for (auto& soul: heaven_capacity)
	{
		total += soul;
	}

	for (auto& soul : hell_capacity)
	{
		total += soul;
	}

	return total;
}

int PopulationManager::GetADPopulation(PlaneType plane)
{
	switch (plane)
	{
	case Heaven:
		return population_angel;

	case Hell:
		return population_demon;
	}

	return 0;
}

int PopulationManager::GetADCapacity(PlaneType plane)
{
	switch (plane)
	{
	case Heaven:
		return capacity_angel;

	case Hell:
		return capacity_demon;
	}

	return 0;
}

bool PopulationManager::GetZoneFull(PlaneType plane, ZoneType zone)
{
	switch (plane)
	{
	case Heaven:
		return heaven_full[(int)zone - 1];

	case Hell:
		return hell_full[(int)zone - 1];
	}
}

/// <summary>
/// Checks if the population exceeds the current capacity
/// </summary>
/// <param name="plane">Heaven or Hell</param>
/// <param name="zone">ZoneType of the population to be checked</param>
/// <returns>True if population exceeds current capacity, False otherwise</returns>
bool PopulationManager::PopulationCheck(PlaneType plane, ZoneType zone)
{
	switch (plane)
	{
	case Heaven:
		if (heaven_population[(int)zone - 1] > heaven_capacity[(int)zone - 1])
		{
			return true;
		}
		else
		{
			return false;
		}

	case Hell:
		if (hell_population[(int)zone - 1] > hell_capacity[(int)zone - 1])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void PopulationManager::IncrementReligions(PlaneType plane, int pop_inc)
{
	int count = 0;
	switch (plane)
	{
	case Heaven:
		for (int i = 0; i < pop_inc; i++)
		{
			srand(std::time(nullptr) + count);
			int belief_1 = rand() % 3;
			if (belief_1 == 2) { belief_1 = 3; }
			heaven_religion_spread[belief_1] += 1;
			heaven_religion_spread[(rand() % 2) + 4] += 1;
			heaven_religion_spread[(rand() % 2) + 6] += 1;
			count++;
		}
		break;
	case Hell:
		for (int i = 0; i < pop_inc; i++)
		{
			srand(std::time(nullptr) + count);
			hell_religion_spread[rand() % 3] += 1;
			hell_religion_spread[(rand() % 2) + 4] += 1;
			hell_religion_spread[(rand() % 2) + 6] += 1;
			count++;
		}
		break;
	}
	/*
	for (int i = 0; i < 8; i++)
	{
		std::cout << heaven_religion_spread[i] << " " << hell_religion_spread[i] << std::endl;
	}
	std::cout << "--" << std::endl;*/
}

int PopulationManager::GetReligiousSpread(PlaneType plane, int rel)
{
	switch (plane)
	{
	case Heaven:
		return heaven_religion_spread[rel];
		break;
	case Hell:
		return hell_religion_spread[rel];
		break;
	}
}
