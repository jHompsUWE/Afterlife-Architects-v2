#include "pch.h"
#include "UIWindowAudioSettings.h"

UIWindowAudioSettings::UIWindowAudioSettings(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text, std::string _filepath,
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

    for (int i = 0; i < 3; i++)
    {
        buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x + 32.5, window_pos.y + 52 + 55 * i),
            DataManager::GetD3DDevice(), "arrow_left",
            AL::EventType::event_ui, actions[2 * i], 0, Vector2(1.0, 1.0)));
        buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x + 242.5, window_pos.y + 52 + 55 * i),
            DataManager::GetD3DDevice(), "arrow_right",
            AL::EventType::event_ui, actions[2 * i + 1], 0, Vector2(1.0, 1.0)));
        image_vec.push_back(new ImageGO2D("slider_back", DataManager::GetD3DDevice()));
        image_vec[2 * i]->SetPos(Vector2(window_pos.x + 137.5, window_pos.y + 53 + 55 * i));
        image_vec[2 * i]->SetScale(Vector2(window_res.x / 352, 1));
        image_vec.push_back(new ImageGO2D("slider_handle", DataManager::GetD3DDevice()));
        image_vec[2 * i + 1]->SetPos(Vector2(window_pos.x + 208.8, window_pos.y + 53 + 55 * i));
        image_vec[2 * i + 1]->SetScale(Vector2(1, 1));

        sliders.push_back(new UISlider(100, 1));
        sliders.push_back(new UISlider(100, 1));
    }

    AL::NewEventManager::AddEventReceiver(false, this, AL::EventType::event_ui);
}

UIWindowAudioSettings::~UIWindowAudioSettings()
{
    for (auto image : image_vec)
    {
        delete image;
    }
}

void UIWindowAudioSettings::update(GameData* _gameData, Vector2& _mousePosition)
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

const bool& UIWindowAudioSettings::ReceiveEvents(const AL::Event& al_event)
{
    if (al_event.type == AL::event_ui)
    {
        switch (al_event.input.action)
        {
        case AL::UI::audio_master_left:
            updateSlider(0, -5);
            break;
        case AL::UI::audio_master_right:
            updateSlider(0, 5);
            break;
        case AL::UI::audio_music_left:
            updateSlider(1, -5);
            break;
        case AL::UI::audio_music_right:
            updateSlider(1, 5);
            break;
        case AL::UI::audio_sound_left:
            updateSlider(2, -5);
            break;
        case AL::UI::audio_sound_right:
            updateSlider(2, 5);
            break;
        }
    }

    return UIWindow::ReceiveEvents(al_event);
}

/// <summary>
/// Update position of slider visual based off slider percentage within slider class
/// </summary>
void UIWindowAudioSettings::updateSlider(int slider_index, int perc_change)
{
    // Update position of slider handle
    image_vec[2 * slider_index + 1]->SetPos(Vector2(sliders[slider_index]->updateSlider(perc_change, window_res.x), 0) + image_vec[2 * slider_index + 1]->GetPos());
    AL::NewEventManager::GenerateEventSt(AL::event_sound_volume, slider_index, sliders[slider_index]->getPercent());
}

void UIWindowAudioSettings::render(DrawData2D* _drawData)
{
    if (!is_visible)
    {
        inside = false;
        return;
    }

    windowBackGround->Draw(_drawData);

    for (auto button : buttons)
    {
        button->render(_drawData);
    }
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

void UIWindowAudioSettings::reSize(Vector2 game_res)
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