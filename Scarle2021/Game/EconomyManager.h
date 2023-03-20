#pragma once
#include <memory>
#include "GameData.h"

class EconomyManager
{
public:
	EconomyManager();
	~EconomyManager() = default;

	void Tick(GameData* game_data);
	void SetMoney(float new_amount) { money = new_amount; }
	void IncrementMoney(float money_increase) { money += money_increase; }
	float GetMoney() { return money; }
	int GetYear() { return year; }

private:
	bool CallEverySeconds(float dt, float time_interval);

	float money;
	int year;

	float timer;
};

