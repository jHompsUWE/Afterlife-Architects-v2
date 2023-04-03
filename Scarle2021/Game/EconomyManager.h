#pragma once
#include <memory>
#include "GameData.h"
#include "PopulationManager.h"
#include <iostream>

class EconomyManager
{
public:
	EconomyManager(std::shared_ptr<PopulationManager> _pop_manager);
	~EconomyManager() = default;

	void Tick(GameData* game_data);
	void SetMoney(float new_amount) { money = new_amount; }
	void IncrementMoney(float money_increase) { money += money_increase; }
	void SetTotalRoads(int t_roads) { total_roads = t_roads; };
	float GetMoney() { return money; }
	int GetYear() { return year; }
	void SetRaD(float _rad) { rad_prod_percent = _rad; };
	float GetRaD() { return rad_prod_percent; };

	bool PurchaseStructure(int cost);

private:
	void YearUpdate();
	bool CallEverySeconds(float dt, float time_interval);

	std::shared_ptr<PopulationManager> pop_manager;

	int prev_year_population;
	float money;
	int year;
	float time_per_year = 2;

	int total_roads;
	float cost_per_road = 1.0f;

	float timer;
	float money_per_soul = 0.1f;

	float rad_prod_percent = 50;
};

