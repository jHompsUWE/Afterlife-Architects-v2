#pragma once
#include "StateTemplate.h"
#include "Button.hpp"

class MainMenu : public StateTemplate
{
public:
	MainMenu();
	~MainMenu() override;

	bool init() override;
	
	//Update Cycles
	void Update(GameData* game_data) override;
	void ScaledUpdate(GameData* game_data, float& scaled_dt) override;
	void LateUpdate(GameData* game_data) override;

	//Events
	void GetEvents(const AL::Event& al_event) override;
	
	//Renders
	void Render2D(DrawData2D* draw_data2D) override;
	void Render3D(DrawData* draw_data) override;

private:
	void ResizeUI() const;

	void MoveOffScreen();
	void MoveOnScreen() const;

	//Button vector
	std::vector<UIButtonInterFace*> buttons;

	//Init
	bool do_once = true;
	
	ImageGO2D* main_menu_bg = nullptr;
};

