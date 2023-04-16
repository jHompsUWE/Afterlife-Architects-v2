#pragma once
#include <vector>

#include "DrawData2D.h"
#include "GameData.h"
#include "SimpleMath.h"
#include "UIWindow.h"


class HierarchyManager : public AL::EventReceiver
{
public:
	HierarchyManager();
	~HierarchyManager() override;
	
	void Update(GameData* game_data, Vector2& mouse_position);
	void Render(DrawData2D* draw_data);

	void AddToHierarchy(UIWindow* window);

	void OpenCloseWindow(UIWindow* window);

	void MoveAllToFront() const;

	void MoveToFrontIfInside();
	
	const bool& ReceiveEvents(const AL::Event& al_event) override;

	void MoveInFront(UIWindow* window);
	
private:
	std::vector<UIWindow*> windows {};

	Vector2 mouse_pos {0,0};
};
