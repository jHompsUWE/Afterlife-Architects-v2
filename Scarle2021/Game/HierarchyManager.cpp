#include "pch.h"
#include "HierarchyManager.h"

HierarchyManager::HierarchyManager() = default;

HierarchyManager::~HierarchyManager() = default;

void HierarchyManager::Update(GameData* game_data, SimpleMath::Vector2& mouse_position)
{
    for (auto& window : windows)
    {
        window->update(game_data, mouse_position);
    }
}

void HierarchyManager::Render(DrawData2D* draw_data)
{
    for (auto& window : windows)
    {
        window->render(draw_data);
    }
}

void HierarchyManager::AddToHierarchy(UIWindow* window)
{
    windows.emplace_back(window);
}
