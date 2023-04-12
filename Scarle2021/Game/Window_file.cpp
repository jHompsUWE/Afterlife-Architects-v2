#include "pch.h"
#include "Window_file.h"

Window_file::Window_file(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text, std::string _filepath,
    Vector2 _setScale)
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
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+20),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::file_Save, 0,Vector2(5,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+50),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::file_load, 0,Vector2(5,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+75),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::file_load_scenario, 0,Vector2(5,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+105),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::file_delete_game, 0,Vector2(5,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+135),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::file_new_game, 0,Vector2(5,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+195),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::file_exit_game, 0,Vector2(5,0.7)));
}

Window_file::~Window_file()
{
}

void Window_file::update(GameData* _gameData, Vector2& _mousePosition)
{
}

void Window_file::render(DrawData2D* _drawData)
{
    if(is_visible)
    {
        for (auto button : buttons)
        {
            button->render(_drawData);
        }
        windowBackGround->Draw(_drawData);
    }
   
}

void Window_file::set_postion(Vector2& _new_pos)
{
}

void Window_file::setScale(Vector2& _newScale)
{
    UIWindow::setScale(_newScale);
}

Vector2& Window_file::getPosition()
{
    return UIWindow::getPosition();
}

Vector2& Window_file::getButtonRes()
{
    return UIWindow::getButtonRes();
}

void Window_file::reSize(Vector2 game_res)
{
    UIWindow::reSize(game_res);
}
