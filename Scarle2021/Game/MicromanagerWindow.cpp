#include "pch.h"
#include "MicromanagerWindow.h"
#include <iostream>

#include "DataManager.h"


MicromanagerWindow::MicromanagerWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text,
    std::string _filepath, Vector2 _setScale, std::shared_ptr<EconomyManager> _economy_manager) : UIWindow()
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

    image_vec.push_back(new ImageGO2D("slider_back", DataManager::GetD3DDevice()));
    image_vec[0]->SetPos(Vector2(window_pos.x + 177.5, window_pos.y + 33));
    image_vec[0]->SetScale(Vector2(1, 1));
    image_vec.push_back(new ImageGO2D("slider_handle", DataManager::GetD3DDevice()));
    image_vec[1]->SetPos(Vector2(window_pos.x + 177.5, window_pos.y + 33));
    image_vec[1]->SetScale(Vector2(1, 1));
    
    rad_slider = new UISlider(50,1.0f);

    economy_manager = _economy_manager;

    AL::NewEventManager::AddEventReceiver(false, this, AL::EventType::event_ui);
}

MicromanagerWindow::~MicromanagerWindow()
{
    for (auto image : image_vec)
    {
        delete image;
    }
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

    inside = isInside(mouse_pos);

    //if clicked updates pos and scale for window drag  
    if (toggle_click && inside)
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
    if (!is_visible)
    {
        inside = false;
        return;
    }

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

const bool& MicromanagerWindow::ReceiveEvents(const AL::Event& al_event)
{
    if(al_event.type == AL::event_ui)
    {
        if (al_event.input.action == AL::UI::micro_manager_arrow_l)
        {
            updateSlider(-5);
        }
        else if (al_event.input.action == AL::UI::micro_manager_arrow_r)
        {
            updateSlider(5);
        }
    }
    
    return UIWindow::ReceiveEvents(al_event);
}

/// <summary>
/// Update position of slider visual based off slider percentage within slider class
/// </summary>
void MicromanagerWindow::updateSlider(int perc_change)
{
    // Update position of slider handle
    image_vec[1]->SetPos(Vector2(rad_slider->updateSlider(perc_change, window_res.x), 0) + image_vec[1]->GetPos());

    // Update text and classes linked with RAD values
    int slider_percent = rad_slider->getPercent();
    text_vec[0]->ChangeString(std::to_string(100-slider_percent) + "%");
    text_vec[1]->ChangeString(std::to_string(slider_percent) + "%");
    economy_manager->SetRaD(slider_percent);
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

