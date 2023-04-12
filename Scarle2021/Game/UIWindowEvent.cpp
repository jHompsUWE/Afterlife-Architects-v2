#include "pch.h"
#include "UIWindowEvent.h"

UIWindowEvent::UIWindowEvent(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text,
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
    
    //setup button text
    text_vec.push_back(new TextGO2D(_text));
    text_vec.back()->SetColour(Color((float*)&Colors::Black));
    text_vec.back()->SetPos(Vector2(window_pos.x+ 35,window_pos.y+ 95));
    text_vec.back()->SetScale(Vector2(0.3,0.3));
    
    //window buttons.....................
    buttons.push_back(new Button<AL::BuildSys::Section, StructureType>(Vector2(window_pos.x+80,window_pos.y+50),
        DataManager::GetD3DDevice(),"KS_Heaven_3x3",
        AL::EventType::event_build_sys,AL::BuildSys::structure, KarmaStation_T1,Vector2(0.4,0.4)));
}

UIWindowEvent::~UIWindowEvent()
{
   
}

void UIWindowEvent::update(GameData* _gameData, Vector2& _mousePosition)
{
    UIWindow::update(_gameData,_mousePosition);
}

void UIWindowEvent::render(DrawData2D* _drawData)
{
    UIWindow::render(_drawData);
}

void UIWindowEvent::set_postion(Vector2& _new_pos)
{
}

void UIWindowEvent::setScale(Vector2& _newScale)
{
    UIWindow::setScale(_newScale);
}

void UIWindowEvent::setString(std::string text_x,text_index index)
{
    text_vec[(int)index]->ChangeString(text_x);
}

Vector2& UIWindowEvent::getPosition()
{
    return window_pos;
}

Vector2& UIWindowEvent::getButtonRes()
{
    return window_res;
}

void UIWindowEvent::reSize(Vector2 game_res)
{
    UIWindow::reSize(game_res);
}
