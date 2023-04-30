#include "pch.h"
#include "Window_Global.h"

Window_Global::Window_Global(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text,
                                       std::string _filepath, Vector2 _setScale):UIWindow()
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
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+25),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::global_audio_settings_window, 0,Vector2(5,0.7), true));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+58),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::global_tempo_window, 0,Vector2(5,0.7), true));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+88),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::global_bad_things_window, 0,Vector2(5,0.7), true));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+118),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::global_misc_options_window, 0,Vector2(5,0.7), true));

    AL::EventManager::RemoveEventReceiver(this);
    AL::EventManager::AddEventReceiver(true, this, AL::EventType::event_cursor_interact);
}

Window_Global::~Window_Global()
{
    AL::EventManager::RemoveEventReceiver(this);
}

void Window_Global::update(GameData* _gameData, Vector2& _mousePosition)
{
    if(!is_visible) return;
    
    //updates buttons
    for (const auto& button : buttons)
    {
        button->update(_gameData);
        if(button->close_window)
        {
            setVisibility(false);
            button->close_window = false;
        }
    }
 
    //window background
    windowBackGround->Tick(_gameData);

    inside = isInside(_mousePosition);
}

void Window_Global::render(DrawData2D* _drawData)
{
    if (!is_visible)
    {
        inside = false;
        return;
    }

    for (auto button : buttons)
    {
        button->render(_drawData);
    }
    windowBackGround->Draw(_drawData);
}