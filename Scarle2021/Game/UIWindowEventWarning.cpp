#include "pch.h"
#include "UIWindowEventWarning.h"

UIWindowEventWarning::UIWindowEventWarning(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text, std::string _filepath,
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

    //setup button text
    text_vec.push_back(new TextGO2D("some thing about something"));
    text_vec.back()->SetColour(Color((float*)&Colors::White));
    text_vec.back()->SetPos(Vector2(window_pos.x+ 25,window_pos.y+ 25));
    text_vec.back()->SetScale(Vector2(0.5,0.5));

    //window buttons.....................
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+118,window_pos.y+160),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::event_activated, 0,Vector2(2.4,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x+255,window_pos.y+160),
        DataManager::GetD3DDevice(),"Green",
         AL::EventType::event_ui,AL::UI::event_canceled, 0,Vector2(2.4,0.7)));
}

UIWindowEventWarning::~UIWindowEventWarning()
{
}

void UIWindowEventWarning::update(GameData* _gameData, Vector2& _mousePosition)
{
    UIWindow::update(_gameData, _mousePosition);
}

void UIWindowEventWarning::render(DrawData2D* _drawData)
{
    if(is_visible)
    {
        
        for (auto button : buttons)
        {
            button->render(_drawData);
        }
        windowBackGround->Draw(_drawData);
        for (auto text : text_vec)
        {
            text->Draw(_drawData);
        }
    }
}

void UIWindowEventWarning::set_postion(Vector2& _new_pos)
{
}

void UIWindowEventWarning::setScale(Vector2& _newScale)
{
    UIWindow::setScale(_newScale);
}

void UIWindowEventWarning::setString(std::string text_x)
{
    text_vec[0]->ChangeString(text_x);
}

Vector2& UIWindowEventWarning::getPosition()
{
    return UIWindow::getPosition();
}

Vector2& UIWindowEventWarning::getButtonRes()
{
    return UIWindow::getButtonRes();
}

void UIWindowEventWarning::reSize(Vector2 game_res)
{
    UIWindow::reSize(game_res);
}