#include "pch.h"
#include "UIWindowBadThings.h"

UIWindowBadThings::UIWindowBadThings(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text, std::string _filepath,
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
         AL::EventType::event_ui,AL::UI::no_bad_things, 0,Vector2(5,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+58),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::birds_of_paradise, 0,Vector2(5,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+85),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::rats_out_of_hell, 0,Vector2(5,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+115),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::my_blue_heaven, 0,Vector2(5,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+145),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::hell_freezes_over, 0,Vector2(5,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+175),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::heaven_nose, 0,Vector2(5,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+205),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::hell_in_a_hand_basket, 0,Vector2(5,0.7)));

    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+235),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::paradise_pair_of_dice, 0,Vector2(5,0.7)));

    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+130,window_pos.y+265),
     DataManager::GetD3DDevice(),"Green",
      AL::EventType::event_ui,AL::UI::disco_infernal, 0,Vector2(5,0.7)));
}

UIWindowBadThings::~UIWindowBadThings()
{
}

void UIWindowBadThings::update(GameData* _gameData, Vector2& _mousePosition)
{
    UIWindow::update(_gameData, _mousePosition);
}

void UIWindowBadThings::render(DrawData2D* _drawData)
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
    for (auto t : text_vec)
    {
        t->Draw(_drawData);
    }
}

void UIWindowBadThings::set_postion(Vector2& _new_pos)
{
    UIWindow::setPostion(_new_pos);
}

void UIWindowBadThings::setScale(Vector2& _newScale)
{
    UIWindow::setScale(_newScale);
}

Vector2& UIWindowBadThings::getPosition()
{
    return UIWindow::getPosition();
}

Vector2& UIWindowBadThings::getButtonRes()
{
    return UIWindow::getButtonRes();
}

void UIWindowBadThings::reSize(Vector2 game_res)
{
    UIWindow::reSize(game_res);
}