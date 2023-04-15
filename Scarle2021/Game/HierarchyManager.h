#pragma once
#include <vector>

#include "DrawData2D.h"
#include "GameData.h"
#include "SimpleMath.h"
#include "UIWindow.h"


class HierarchyManager
{
public:
	HierarchyManager();
	~HierarchyManager();
	
	void Update(GameData* game_data, SimpleMath::Vector2& mouse_position);
	void Render(DrawData2D* draw_data);

	void AddToHierarchy(UIWindow* window);
	
private:
	std::vector<UIWindow*> windows {};
};
