#include "pch.h"
#include "HierarchyManager.h"

HierarchyManager::HierarchyManager()
{
    AL::NewEventManager::AddEventReceiver(true, this, AL::EventType::event_cursor_interact);
}

HierarchyManager::~HierarchyManager()
{
    AL::NewEventManager::RemoveEventReceiver(this);
}

void HierarchyManager::Update(GameData* game_data, Vector2& mouse_position)
{
    for (const auto& window : windows)
    {
        window->update(game_data, mouse_position);
    }
}

void HierarchyManager::Render(DrawData2D* draw_data)
{
    for (const auto& window : windows)
    {
        window->render(draw_data);
    }
}

void HierarchyManager::AddToHierarchy(UIWindow* window)
{
    windows.emplace_back(window);
}

void HierarchyManager::OpenCloseWindow(UIWindow* window)
{
    window->setVisibility(!window->getVisibility());
    MoveInFront(window);
}

void HierarchyManager::MoveAllToFront() const
{
    for (const auto& window : windows)
    {
        window->MoveInFront();
    }
}

void HierarchyManager::MoveToFrontIfInside()
{
    for (auto it = windows.rbegin(); it != windows.rend(); ++it)
    {
        if((*it)->IsCursorInsideWindow())
        {
            MoveInFront(*it);
            break;
        }
    }
}

const bool& HierarchyManager::ReceiveEvents(const AL::Event& al_event)
{
    if(al_event.cursor_interact.active) MoveToFrontIfInside();
    return false;
}

void HierarchyManager::MoveInFront(UIWindow* window)
{
    auto it = std::find(windows.begin(), windows.end(), window);
    
    if(it != windows.end() && window->getVisibility())
    {
        window->MoveInFront();
        std::rotate(it, it + 1, windows.end());
    }
}
