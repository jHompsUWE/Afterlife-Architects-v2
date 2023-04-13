#include "pch.h"
#include "GraphviewWindow.h"

#include <iostream>

#include "DataManager.h"


GraphviewWindow::GraphviewWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text,
    std::string _filepath, Vector2 _setScale, std::shared_ptr<PopulationManager> pop_manager) : population_manager(pop_manager)
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

    for (int i = 0; i < 16; i++)
    {
        belief_bars_vec.push_back(new ImageGO2D("green", DataManager::GetD3DDevice()));
        belief_bars_vec[i]->SetPos(Vector2(window_pos.x + 95, window_pos.y + 45 + 11 * i));
        belief_bars_vec[i]->SetScale(Vector2(0.2, 0.2));
    }

    for (int i = 0; i < 14; i++)
    {
        population_bars_vec.push_back(new ImageGO2D("green", DataManager::GetD3DDevice()));
        population_bars_vec[i]->SetPos(Vector2(window_pos.x + 85, window_pos.y + 67 + 11 * i));
        population_bars_vec[i]->SetScale(Vector2(0.275, 0.275));
    }

    //button vector
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x + 327, window_pos.y + 36),
        DataManager::GetD3DDevice(), "green",
        AL::EventType::event_ui, AL::UI::graphview_bel, 0, Vector2(0.6, 0.6)));
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x + 327, window_pos.y + 64),
        DataManager::GetD3DDevice(), "green",
        AL::EventType::event_ui, AL::UI::graphview_rel, 0, Vector2(0.6, 0.6)));
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(window_pos.x + 327, window_pos.y + 175),
        DataManager::GetD3DDevice(), "green",
        AL::EventType::event_ui, AL::UI::graphview_pop, 0, Vector2(0.6, 0.6)));

    AL::NewEventManager::AddEventReceiver(this);
}

GraphviewWindow::~GraphviewWindow()
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
    switch (cur_graph)
    {
    case Religion:
        break;
    case Belief:
        updateBeliefSpread();
        updateBeliefVisual();
        break;
    case Population:
        updatePopulationVisual();
        break;
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
        for (auto& image : belief_bars_vec)
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
    if (!is_visible) return;

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
        //image_vec[2]->Draw(_drawData);
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

void GraphviewWindow::ReceiveEvents(const AL::Event& al_event)
{
    switch (al_event.type)
    {
    case AL::event_input:
        break;

    case AL::event_ui:
        if (al_event.ui.action == AL::UI::graphview_bel)
        {
            cur_graph = Belief;
        }
        else if (al_event.ui.action == AL::UI::graphview_pop)
        {
            cur_graph = Population;
        }
        else if (al_event.ui.action == AL::UI::graphview_rel)
        {
            cur_graph = Religion;
        }
        break;

    case AL::event_cursor_interact:
        //Saves the state of the action
        if (al_event.cursor_interact.action == AL::Cursor::button_input1)
        {
            toggle_click = al_event.cursor_interact.active;
        }
        break;

    default:
        break;
    }
}

void GraphviewWindow::set_postion(Vector2& _new_pos)
{
    window_pos = _new_pos;
}

void GraphviewWindow::set_scale(Vector2& _newScale)
{
}

Vector2& GraphviewWindow::getPosition()
{
    return window_pos;
}

Vector2& GraphviewWindow::getButtonRes()
{
    return window_res;
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

bool GraphviewWindow::isInside(Vector2& point) const
{
    //checks bounding box of UI window
    if (point.x >= window_pos.x && point.x <= (window_pos.x + window_res.x) &&
        point.y >= window_pos.y && point.y <= (window_pos.y + window_res.y))
        return true;

    return false;
}

void GraphviewWindow::updateBeliefSpread()
{
    if (population_manager->GetTotalSouls() == 0) { return; }
    float total_souls = (float)population_manager->GetTotalSouls();

    float HAHA = (population_manager->GetReligiousSpread(Heaven, 0) + population_manager->GetReligiousSpread(Hell, 0)) / total_souls;
    float HOHO = (population_manager->GetReligiousSpread(Heaven, 1) + population_manager->GetReligiousSpread(Hell, 1)) / total_souls;
    float OPRA = (population_manager->GetReligiousSpread(Hell, 2)) / total_souls;
    float OCRA = (population_manager->GetReligiousSpread(Heaven, 3)) / total_souls;
    float ALF = (population_manager->GetReligiousSpread(Heaven, 4) + population_manager->GetReligiousSpread(Hell, 4)) / total_souls;
    float RALF = (population_manager->GetReligiousSpread(Heaven, 5) + population_manager->GetReligiousSpread(Hell, 5)) / total_souls;
    float SUMA = (population_manager->GetReligiousSpread(Heaven, 6) + population_manager->GetReligiousSpread(Hell, 6)) / total_souls;
    float SUSA = (population_manager->GetReligiousSpread(Heaven, 7) + population_manager->GetReligiousSpread(Hell, 7)) / total_souls;

    // HAHAALFSUMA
    belief_amounts[0] = HAHA * ALF * SUMA;
    // HOHOALFSUMA
    belief_amounts[1] = HOHO * ALF * SUMA;
    // OPRAALFSUMA
    belief_amounts[2] = OPRA * ALF * SUMA;
    // OCRAALFSUMA
    belief_amounts[3] = OCRA * ALF * SUMA;
    // HAHARALFSUMA
    belief_amounts[4] = HAHA * RALF * SUMA;
    // HOHORALFSUMA
    belief_amounts[5] = HOHO * RALF * SUMA;
    // OPRARALFSUMA
    belief_amounts[6] = OPRA * RALF * SUMA;
    // OCRARALFSUMA
    belief_amounts[7] = OCRA * RALF * SUMA;
    // HAHAALFSUSA
    belief_amounts[8] = HAHA * ALF * SUSA;
    // HOHOALFSUSA
    belief_amounts[9] = HOHO * ALF * SUSA;
    // OPRAALFSUSA
    belief_amounts[10] = OPRA * ALF * SUSA;
    // OCRAALFSUSA
    belief_amounts[11] = OCRA * ALF * SUSA;
    // HAHARALFSUSA
    belief_amounts[12] = HAHA * RALF * SUSA;
    // HOHORALFSUSA
    belief_amounts[13] = HOHO * RALF * SUSA;
    // OPRARALFSUSA
    belief_amounts[14] = OPRA * RALF * SUSA;
    // OCRARALFSUSA
    belief_amounts[15] = OCRA * RALF * SUSA;
}

void GraphviewWindow::updateBeliefVisual()
{
    for (int i = 0; i < 16; i++)
    {
        belief_bars_vec[i]->SetPos(Vector2(window_pos.x + (95 + ((belief_amounts[i] * 2) * 98.5)) * window_res.x / 352, window_pos.y + (45 + 11 * i) * window_res.y / 316));
        belief_bars_vec[i]->SetScale(Vector2((belief_amounts[i] * 2) * 4.2 * window_res.x / 352, 0.2 * window_res.y / 316));
    }
}

void GraphviewWindow::updatePopulationVisual()
{
    for (int i = 0; i < 14; i++)
    {
        float percent_val = population_manager->GetZonePopulation(PlaneType(i % 2), ZoneType(1 + (int)i / 2)) / (float)population_manager->GetTotalSouls();
        population_bars_vec[i]->SetPos(Vector2(window_pos.x + (85 + (103.5 * percent_val)) * window_res.x / 352, window_pos.y + (67 + 11 * i) * window_res.y / 316));
        population_bars_vec[i]->SetScale(Vector2(percent_val * 4.2 * window_res.x / 352, 0.275 * window_res.y / 316));
    }
}