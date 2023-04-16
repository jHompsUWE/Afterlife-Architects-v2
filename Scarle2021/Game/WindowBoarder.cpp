#include "pch.h"
#include "WindowBoarder.h"

WindowBoarder::WindowBoarder(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text,
    std::string _filepath, Vector2 _setScale)
{
    //setup for window background
    windowBackGround = new ImageGO2D(_filepath, _d3dDevice);
    windowBackGround->SetOrigin(Vector2(0, 0));
    windowBackGround->SetScale(Vector2(_setScale));

    //sets window res
    window_res = Vector2(windowBackGround->GetRes().x
        * windowBackGround->GetScale().x, windowBackGround->GetRes().y
        * windowBackGround->GetScale().y);

    // sets window pos
    window_pos = _windowPosition;
    windowBackGround->SetPos(window_pos);

    //window buttons.....................
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+25,window_pos.y+10),
        DataManager::GetD3DDevice(),"Green",
        AL::EventType::event_ui,AL::UI::window_file, 0,Vector2(1.5,1), true));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+100,window_pos.y+10),
        DataManager::GetD3DDevice(),"Green",
        AL::EventType::event_ui,AL::UI::window_global, 0,Vector2(1.5,1), true));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+200,window_pos.y+10),
        DataManager::GetD3DDevice(),"Green",
        AL::EventType::event_ui,AL::UI::window_window, 0,Vector2(1.5,1), true));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+300,window_pos.y+10),
        DataManager::GetD3DDevice(),"Green",
        AL::EventType::event_ui,AL::UI::window_tutorial, 0,Vector2(1.5,1), true));

    AL::NewEventManager::RemoveEventReceiver(this);
    AL::NewEventManager::AddEventReceiver(true, this, AL::EventType::event_cursor_interact);
}

WindowBoarder::~WindowBoarder()
{
    AL::NewEventManager::RemoveEventReceiver(this);
}

void WindowBoarder::update(GameData* _gameData, Vector2& _mousePosition)
{
}

void WindowBoarder::render(DrawData2D* _drawData)
{
    for (auto button : buttons)
    {
        button->render(_drawData);
    }
    windowBackGround->Draw(_drawData);
}
