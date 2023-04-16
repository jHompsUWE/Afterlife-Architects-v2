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

// Game Cycle ----------------------------------------------------------------------------------------------------------


void HierarchyManager::Update(GameData* game_data, Vector2& mouse_position) const
{
    //Updates each window in the correct order
    for (const auto& window : windows)
    {
        window->update(game_data, mouse_position);
    }
}

void HierarchyManager::Render(DrawData2D* draw_data) const
{
    //Render each window in the correct order
    for (const auto& window : windows)
    {
        window->render(draw_data);
    }
}

// Hierarchy subscription/unsubscription -------------------------------------------------------------------------------

void HierarchyManager::AddToHierarchy(UIWindow* window)
{
    windows.emplace_back(window);
}

void HierarchyManager::RemoveFromHierarchy(UIWindow* window)
{
    windows.erase(std::remove(windows.begin(), windows.end(), window), windows.end());
}

// Open / Close --------------------------------------------------------------------------------------------------------

void HierarchyManager::OpenCloseWindow(UIWindow* window)
{
    //Toggles window open/close and moves it on top if opened
    window->setVisibility(!window->getVisibility());
    MoveInFront(window);
}

// Layering ------------------------------------------------------------------------------------------------------------

void HierarchyManager::MoveInFront(UIWindow* window)
{
    //Finds the current iterator of the window in the vector
    auto it = std::find(windows.begin(), windows.end(), window);

    //If it is valid and the window is visible
    if(it != windows.end() && window->getVisibility())
    {
        //moves it in front
        window->MoveInFront();
        //Rotates the vector so that now this window is the front
        std::rotate(it, it + 1, windows.end());
    }
}

void HierarchyManager::MoveAllToFront() const
{
    //Moves all windows at the front, used in initialization
    for (const auto& window : windows)
    {
        window->MoveInFront();
    }
}

void HierarchyManager::MoveToFrontIfInside()
{
    //checks each window if they have the cursor inside them
    for (auto it = windows.rbegin(); it != windows.rend(); ++it)
    {
        //If yes, moves the windows on front updating the hierarchy 
        if((*it)->IsCursorInsideWindow())
        {
            MoveInFront(*it);
            break;
        }
    }
}

// Extra ---------------------------------------------------------------------------------------------------------------

const bool& HierarchyManager::ReceiveEvents(const AL::Event& al_event)
{
    //Calls MoveToFrontIfInside if mouse is pressed
    if(al_event.cursor_interact.active) MoveToFrontIfInside();
    return false;
}

void HierarchyManager::ResizeAll() const
{
    const auto screen_size = Vector2(*DataManager::GetRES().first, *DataManager::GetRES().second);

    //Resizes all windows in the hierarchy 
    for (const auto& window : windows)
    {
        window->reSize(screen_size);
    }
}
