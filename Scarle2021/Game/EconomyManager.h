#pragma once
#include <memory>
#include "GameData.h"
#include "PopulationManager.h"
#include "EventManager.h"
#include <iostream>

class EconomyManager
{
public:
	EconomyManager(std::shared_ptr<PopulationManager> _pop_manager);
	~EconomyManager() = default;

	void Tick(GameData* game_data);
	void SetTotalRoads(int t_roads) { total_roads = t_roads; };
	void SetStructureCount(int s_count) { total_structures = s_count; };

	float GetMoney() { return money; }
	void IncrementMoney(float money_increase) { money += money_increase; }
	void SetMoney(float new_amount) { money = new_amount; }

	int GetYear() { return year; }

	void SetRaD(float _rad) { rad_prod_percent = _rad; };
	float GetRaD() { return rad_prod_percent; };

	bool PurchaseStructure(int cost);

private:
	void YearUpdate();
	bool CallEverySeconds(float dt, float time_interval);

	std::shared_ptr<PopulationManager> pop_manager;

	// Year variables
	float timer;
	int year;
	float time_per_year = 2;

	// Tracker variables
	int prev_year_population;
	float money;
	int total_structures = 0;
	float rad_prod_percent = 50;

	// Road variables
	int total_roads;
	float cost_per_road = 1.0f;

	float money_per_soul = 0.1f;
};

