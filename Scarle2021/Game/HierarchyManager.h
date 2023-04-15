#pragma once
#include <vector>

#include "EventReceiver.hpp"
#include "GameData.h"
#include "HierarchyInterface.h"

class HierarchyManager
{
public:
	HierarchyManager();
	~HierarchyManager();
	
	void Update(GameData* game_data, SimpleMath::Vector2& mouse_position);
	void Render(DrawData2D* draw_data);

	void AddToHierarchy(HierarchyInterface* window);
	
private:
	std::vector<HierarchyInterface*> windows {};
};
