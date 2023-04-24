#include "pch.h"
#include "GraphviewWindow.h"
#include <cmath>

#include <iostream>

#include "DataManager.h"


GraphviewWindow::GraphviewWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text,
    std::string _filepath, Vector2 _setScale, std::shared_ptr<PopulationManager> pop_manager)
        : UIWindow(), population_manager(pop_manager)
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

    image_vec.push_back(new ImageGO2D("graphview_belief", DataManager::GetD3DDevice()));
    image_vec[0]->SetPos(Vector2(window_pos.x + window_res.x / 2, window_pos.y + window_res.y / 2));
    image_vec[0]->SetScale(Vector2(1, 1));

    image_vec.push_back(new ImageGO2D("graphview_population", DataManager::GetD3DDevice()));
    image_vec[1]->SetPos(Vector2(window_pos.x + window_res.x / 2, window_pos.y + window_res.y / 2));
    image_vec[1]->SetScale(Vector2(1, 1));

    image_vec.push_back(new ImageGO2D("graphview_religion", DataManager::GetD3DDevice()));
    image_vec[2]->SetPos(Vector2(window_pos.x + window_res.x / 2, window_pos.y + window_res.y / 2));
    image_vec[2]->SetScale(Vector2(1, 1));

    for (int i = 0; i < 16; i++)
    {
        belief_bars_vec.push_back(new ImageGO2D("green", DataManager::GetD3DDevice()));
    }

    for (int i = 0; i < 14; i++)
    {
        population_bars_vec.push_back(new ImageGO2D("green", DataManager::GetD3DDevice()));
    }
    
    for (int i = 0; i < 8; i++)
    {
        religion_bars_vec.push_back(new ImageGO2D("green", DataManager::GetD3DDevice()));
    }

    //button vector
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x + 327, window_pos.y + 36),
        DataManager::GetD3DDevice(), "green",
        AL::EventType::event_ui, AL::UI::graphview_rel, 0, Vector2(0.6, 0.6)));
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x + 327, window_pos.y + 64),
        DataManager::GetD3DDevice(), "green",
        AL::EventType::event_ui, AL::UI::graphview_bel, 0, Vector2(0.6, 0.6)));
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x + 327, window_pos.y + 175),
        DataManager::GetD3DDevice(), "green",
        AL::EventType::event_ui, AL::UI::graphview_pop, 0, Vector2(0.6, 0.6)));

    AL::NewEventManager::AddEventReceiver(false, this, AL::EventType::event_ui);
}

GraphviewWindow::~GraphviewWindow()
{
    for (auto image : image_vec)
    {
        delete image;
    }
}

void GraphviewWindow::update(GameData* _gameData, Vector2& _mousePosition)
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
    timer += _gameData->delta_time;
    if (timer >= time_between_updates)
    {
        switch (cur_graph)
        {
        case Religion:
            updateReligionVisual();
            break;
        case Belief:
            updateBeliefVisual();
            break;
        case Population:
            updatePopulationVisual();
            break;
        }
        timer -= time_between_updates;
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
        for (auto& image : belief_bars_vec)
        {
            Vector2 const button_pos = image->GetPos();
            image->SetPos(button_pos - offset);
        }
        for (auto& image : population_bars_vec)
        {
            Vector2 const button_pos = image->GetPos();
            image->SetPos(button_pos - offset);
        }
        for (auto& image : religion_bars_vec)
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

void GraphviewWindow::render(DrawData2D* _drawData)
{
    if (!is_visible)
    {
        inside = false;
        return;
    }

    //renders buttons
    for (const auto& button : buttons)
    {
        button->render(_drawData);
    }
    windowBackGround->Draw(_drawData);

    // render images & texts
    switch (cur_graph)
    {
    case No_Graph:
        break;
    case Population:
        image_vec[1]->Draw(_drawData);
        for (const auto& img : population_bars_vec)
        {
            img->Draw(_drawData);
        }
        break;
    case Religion:
        image_vec[2]->Draw(_drawData);
        for (const auto& img : religion_bars_vec)
        {
            img->Draw(_drawData);
        }
        break;
    case Belief:
        image_vec[0]->Draw(_drawData);
        for (const auto& img : belief_bars_vec)
        {
            img->Draw(_drawData);
        }
        break;
    }
}

bool GraphviewWindow::ReceiveEvents(const AL::Event& al_event)
{
    // Change focused graph based on button input event
    if(al_event.type == AL::EventType::event_ui)
    {
        if (al_event.ui.action == AL::UI::graphview_bel)
        {
            cur_graph = Belief;
            updateBeliefVisual();
        }
        else if (al_event.ui.action == AL::UI::graphview_pop)
        {
            cur_graph = Population;
            updatePopulationVisual();
        }
        else if (al_event.ui.action == AL::UI::graphview_rel)
        {
            cur_graph = Religion;
            updateReligionVisual();
        }
    }
    
    return UIWindow::ReceiveEvents(al_event);
}


void GraphviewWindow::reSize(Vector2 game_res)
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

/// <summary>
/// Updates the belief_amounts and rel_spreads arrays basedon current population manager variables
/// </summary>
void GraphviewWindow::updateBeliefSpread()
{
    // Stop if there are no souls to show
    if (population_manager->GetTotalSouls() == 0) { return; }

    // Get the percentage of each religious section e.g. HAHA, HOHO
    float total_souls = (float)population_manager->GetTotalSouls();
    for (int i = 0; i < 8; i++)
    {
        rel_spreads[i] = (population_manager->GetReligiousSpread(Heaven, i) + population_manager->GetReligiousSpread(Hell, i)) / total_souls;
    }

    // Get the percentage of whole religious beliefs e.g. HAHAALFSUMA
    // rel_spreads[i]: 0 = HAHA, 1 = HOHO, 2 = OPRA, 3 = OCRA, 4 = ALF, 5 =RALF, 6 = SUMA, 7 = SUSA
    float temp;
    for (int i = 0; i < 16; i++)
    {
        temp = 1;
        temp *= rel_spreads[i % 4];

        if (i < 8)
        {
            temp *= rel_spreads[6];
            if (temp < 4) { temp *= rel_spreads[4]; }
            else { temp *= rel_spreads[5]; }
        }
        else
        {
            temp *= rel_spreads[7];
            if (temp < 12) { temp *= rel_spreads[4]; }
            else { temp *= rel_spreads[5]; }
        }

        belief_amounts[i] = temp;
    }
}

/// <summary>
/// Update position and scale of bars for belief distribution
/// </summary>
void GraphviewWindow::updateBeliefVisual()
{
    updateBeliefSpread();
    Vector2 res_factor = window_res / base_resolution;
    for (int i = 0; i < 16; i++)
    {
        Vector2 new_pos;
        new_pos.x = window_pos.x + (95 + ((belief_amounts[i] * 2) * 98.5)) * res_factor.x;
        new_pos.y = window_pos.y + (45 + 11 * i) * res_factor.y;

        Vector2 new_scale;
        new_scale.x = belief_amounts[i] * max_belief_scales.x * res_factor.x;
        new_scale.y = max_belief_scales.y * res_factor.y;

        belief_bars_vec[i]->SetPos(new_pos);
        belief_bars_vec[i]->SetScale(new_scale);
    }
}

/// <summary>
/// Update position and scale of bars for zone population distribution
/// </summary>
void GraphviewWindow::updatePopulationVisual()
{
    float total_pop = (float)population_manager->GetTotalSouls();
    Vector2 res_factor = window_res / base_resolution;
    for (int i = 0; i < 14; i++)
    {
        float percent_val = population_manager->GetZonePopulation(PlaneType(i % 2), ZoneType(1 + (int)i / 2)) / total_pop;
        Vector2 new_pos;
        new_pos.x = window_pos.x + (85 + (103.5 * percent_val)) * res_factor.x;
        new_pos.y = window_pos.y + (67 + 11 * i) * res_factor.y;

        Vector2 new_scale;
        new_scale.x = percent_val * max_population_scales.x * res_factor.x;
        new_scale.y = max_population_scales.y * res_factor.y;

        population_bars_vec[i]->SetPos(new_pos);
        population_bars_vec[i]->SetScale(new_scale);
    }
}

/// <summary>
/// Update position and scale of bars for religion distribution
/// </summary>
void GraphviewWindow::updateReligionVisual()
{
    updateBeliefSpread();
    Vector2 res_factor = window_res / base_resolution;
    for (int i = 0; i < 8; i++)
    {
        float percent_val = rel_spreads[i];
        Vector2 new_pos;
        new_pos.x = window_pos.x + (religion_bars_x[i]) * res_factor.x;
        new_pos.y = window_pos.y + (125 - percent_val * 35) * res_factor.y;

        Vector2 new_scale;
        new_scale.x = max_religion_scales.x * res_factor.x;
        new_scale.y = max_religion_scales.y * percent_val * res_factor.y;

        religion_bars_vec[i]->SetPos(new_pos);
        religion_bars_vec[i]->SetScale(new_scale);
    }
}