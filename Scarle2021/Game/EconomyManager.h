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
	float GetMoney() { return money; }
	int GetYear() { return year; }

	bool PurchaseStructure(int cost);

private:
	void YearUpdate();
	bool CallEverySeconds(float dt, float time_interval);

	std::shared_ptr<PopulationManager> pop_manager;

	int prev_year_population;
	float money;
	int year;
	float time_per_year = 2;

	float timer;
};

