#include "pch.h"
#include "MicromanagerWindow.h"
#include <iostream>

#include "DataManager.h"


MicromanagerWindow::MicromanagerWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text,
    std::string _filepath, Vector2 _setScale, std::shared_ptr<EconomyManager> _economy_manager)
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

    //text vector
    text_vec.push_back(new TextGO2D("50%"));
    text_vec[0]->SetPos(Vector2(window_pos.x + 20, window_pos.y + 50));
    text_vec[0]->SetScale(Vector2(_setScale * 0.2));
    text_vec.push_back(new TextGO2D("50%"));
    text_vec[1]->SetPos(Vector2(window_pos.x + 310, window_pos.y + 50));
    text_vec[1]->SetScale(Vector2(_setScale * 0.2));
    
    //button vector
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x + 62.5, window_pos.y + 32),
        DataManager::GetD3DDevice(), "arrow_left",
        AL::EventType::event_ui, AL::UI::micro_manager_arrow_l, 0, Vector2(1.0,1.0)));
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x + 292.5, window_pos.y + 32),
        DataManager::GetD3DDevice(), "arrow_right",
        AL::EventType::event_ui, AL::UI::micro_manager_arrow_r, 0, Vector2(1.0,1.0)));

    image_vec.push_back(new ImageGO2D("slider_handle", DataManager::GetD3DDevice()));
    image_vec[0]->SetPos(Vector2(window_pos.x + 177.5, window_pos.y + 33));
    image_vec[0]->SetScale(Vector2(1, 1));

    AL::NewEventManager::AddEventReceiver(this);
    economy_manager = _economy_manager;
}

MicromanagerWindow::~MicromanagerWindow()
{
    //deletes pointers
    for (auto button : buttons)
    {
        delete button;
    }

    delete windowBackGround;

    for (auto text : text_vec)
    {
        delete text;
    }

    for (auto image : image_vec)
    {
        delete image;
    }

    AL::NewEventManager::RemoveEventReceiver(this);
}

void MicromanagerWindow::update(GameData* _gameData, Vector2& _mousePosition)
{
    if (!is_visible) return;

    mouse_pos = _mousePosition;

    //updates buttons
    for (const auto& button : buttons)
    {
        button->update(_gameData);
    }
    //updates image
    for (auto image : image_vec)
    {
        image->Tick(_gameData);
    }

    //window background
    windowBackGround->Tick(_gameData);

    //updates texts
    for (auto& text : text_vec)
    {
        text->Tick(_gameData);
    }

    //if clicked updates pos and scale for window drag  
    if (toggle_click && isInside(mouse_pos))
    {

        //new pos on click and drag 
        const Vector2 offset = old_mouse_pos - mouse_pos;
        windowBackGround->SetPos(windowBackGround->GetPos() - offset);
        window_pos = windowBackGround->GetPos();

        //button pos
        for (auto& button : buttons)
        {
            Vector2 const button_pos = button->getPosition();
            button->setPostion(button_pos - offset);
        }

        //image pos
        for (auto& image : image_vec)
        {
            Vector2 const button_pos = image->GetPos();
            image->SetPos(button_pos - offset);
        }
        //text pos
        for (auto& text : text_vec)
        {
            Vector2 const button_pos = text->GetPos();
            text->SetPos(button_pos - offset);
        }
    }
    old_mouse_pos = mouse_pos;
}

void MicromanagerWindow::render(DrawData2D* _drawData)
{
    if (!is_visible) return;

    windowBackGround->Draw(_drawData);
    buttons[0]->render(_drawData);
    buttons[1]->render(_drawData);
    // renders texts
    for (const auto& text : text_vec)
    {
        text->Draw(_drawData);
    }

    for (const auto& image : image_vec)
    {
        image->Draw(_drawData);
    }
}

void MicromanagerWindow::ReceiveEvents(const AL::Event& al_event)
{
    switch (al_event.type)
    {
    case AL::event_input:
        break;

    case AL::event_cursor_interact:
        //Saves the state of the action
        if (al_event.cursor_interact.action == AL::Cursor::button_input1)
        {
            toggle_click = al_event.cursor_interact.active;
        }
        break;
        
    case AL::event_ui:
        if (al_event.input.action == AL::UI::micro_manager_arrow_l)
        {
            if (slider_percent >= 5)
            {
                slider_percent-=5;
                economy_manager->SetRaD(slider_percent);
            }
            updateSlider();
        }
        else if (al_event.input.action == AL::UI::micro_manager_arrow_r)
        {
            if (slider_percent <= 95)
            {
                slider_percent+=5;
                economy_manager->SetRaD(slider_percent);
            }
            updateSlider();
        }
        break;

    default:
        break;
    }
}

void MicromanagerWindow::updateSlider()
{
    Vector2 inital_position = image_vec[0]->GetPos();
    Vector2 position_change = Vector2 ((slider_percent - prev_slider_percent) * 0.02f* slider_radius * window_res.x / 352,0);
    image_vec[0]->SetPos(inital_position + position_change);

    text_vec[0]->ChangeString(std::to_string(100-slider_percent) + "%");
    text_vec[1]->ChangeString(std::to_string(slider_percent) + "%");

    prev_slider_percent = slider_percent;
}

void MicromanagerWindow::set_postion(Vector2& _new_pos)
{
    window_pos = _new_pos;
}

void MicromanagerWindow::set_scale(Vector2& _newScale)
{
}

Vector2& MicromanagerWindow::getPosition()
{
    return window_pos;
}

Vector2& MicromanagerWindow::getButtonRes()
{
    return window_res;
}

void MicromanagerWindow::reSize(Vector2 game_res)
{
    auto& scale = windowBackGround->ReSize(game_res.x, game_res.y);
    //reScales background
    window_res *= scale;
    window_pos *= scale;

    // resize buttons of UI window
    for (const auto& button : buttons)
    {
        button->reSize(game_res);
    }
    for (auto text : text_vec)
    {
        text->ReSize(game_res.x, game_res.y);
    }
    for (auto image : image_vec)
    {
        image->ReSize(game_res.x, game_res.y);
    }
}

bool MicromanagerWindow::isInside(Vector2& point) const
{
    //checks bounding box of UI window
    if (point.x >= window_pos.x && point.x <= (window_pos.x + window_res.x) &&
        point.y >= window_pos.y && point.y <= (window_pos.y + window_res.y))
        return true;

    return false;
}
