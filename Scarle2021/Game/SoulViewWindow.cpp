#include "pch.h"
#include "SoulViewWindow.h"

#include "DataManager.h"

SoulViewWindow::SoulViewWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text,
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

    //text vector
    text_vec.push_back(new TextGO2D(""));
    text_vec[0]->SetPos(Vector2(window_pos.x + 78, window_pos.y + 1.5));
    text_vec[0]->SetScale(Vector2(_setScale * 0.2));
    text_vec.push_back(new TextGO2D(""));
    text_vec[1]->SetPos(Vector2(window_pos.x + 17, window_pos.y + 212));
    text_vec[1]->SetScale(Vector2(_setScale * 0.2));
    text_vec.push_back(new TextGO2D(""));
    text_vec[2]->SetPos(Vector2(window_pos.x + 17, window_pos.y + 223));
    text_vec[2]->SetScale(Vector2(_setScale * 0.2));
    text_vec.push_back(new TextGO2D(""));
    text_vec[3]->SetPos(Vector2(window_pos.x + 17, window_pos.y + 234));
    text_vec[3]->SetScale(Vector2(_setScale * 0.2));
}

SoulViewWindow::~SoulViewWindow()
{
    for (auto image : image_vec)
    {
        delete image;
    }
}

void SoulViewWindow::update(GameData* _gameData, Vector2& _mousePosition)
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

void SoulViewWindow::render(DrawData2D* _drawData)
{
    if (!is_visible)
    {
        inside = false;
        return;
    }

    windowBackGround->Draw(_drawData);

    //renders buttons
    for (const auto& button : buttons)
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

void SoulViewWindow::reSize(Vector2 game_res)
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

void SoulViewWindow::generateRandSoul()
{
    if (population_manager->GetTotalSouls() == 0) { return; }

    srand(std::time(nullptr));

    string temp_string = random_names[std::rand() % 6];
    text_vec[0]->ChangeString(temp_string);

    float heaven_percentage = population_manager->GetTotalSoulsForPlane(Heaven) / (float)population_manager->GetTotalSouls();
    float hell_percentage = population_manager->GetTotalSoulsForPlane(Hell) / (float)population_manager->GetTotalSouls();
    PlaneType hell_or_heaven;
    // If distribution is ONLY hell
    if (heaven_percentage == 0)
    {
        hell_or_heaven = Hell;
    }
    // If distribution is ONLY heaven
    else if (hell_percentage == 0)
    {
        hell_or_heaven = Heaven;
    }
    else
    {
        float rand_perc = (rand() % 100) * 0.01f;
        if (heaven_percentage > rand_perc)
        {
            hell_or_heaven = Heaven;
        }
        else
        {
            hell_or_heaven = Hell;
        }
    }

    switch (hell_or_heaven)
    {
    case Heaven:
        text_vec[1]->ChangeString("Rewarded for " + generateZone(hell_or_heaven));
        break;
    case Hell:
        text_vec[1]->ChangeString("Punished for " + generateZone(hell_or_heaven));
        break;
    }
    text_vec[2]->ChangeString("Years left at location: " + std::to_string(rand() % 950 + 50));
    text_vec[3]->ChangeString("Believes in " + generateBelief(hell_or_heaven) + "ism");
}

string SoulViewWindow::generateBelief(PlaneType plane)
{
    float total_souls_of_plane = population_manager->GetTotalSoulsForPlane(plane);

    float HAHA = population_manager->GetReligiousSpread(plane, 0) / total_souls_of_plane;
    float HOHO = population_manager->GetReligiousSpread(plane, 1) / total_souls_of_plane;
    float ALF = population_manager->GetReligiousSpread(plane, 4) / total_souls_of_plane;
    float SUMA = population_manager->GetReligiousSpread(plane, 6) / total_souls_of_plane;

    int bel_1 = 3;
    int bel_2 = 1;
    int bel_3 = 1;
    float rand_perc;

    rand_perc = (rand() % 100) * 0.01f;
    if (HAHA > rand_perc)
    {
        bel_1 = 0;
    }
    else if (HAHA + HOHO > rand_perc)
    {
        bel_1 = 1;
    }
    else
    {
        switch (plane)
        {
        case Heaven:
            bel_1 = 3;
            break;
        case Hell:
            bel_1 = 2;
            break;
        }
    }

    rand_perc = (rand() % 100) * 0.01f;
    if (ALF > rand_perc)
    {
        bel_2 = 0;
    }
    else
    {
        bel_2 = 1;
    }

    rand_perc = (rand() % 100) * 0.01f;
    if (SUMA > rand_perc)
    {
        bel_3 = 0;
    }
    else
    {
        bel_3 = 1;
    }

    string temp_string = beliefOne[bel_1] + beliefTwo[bel_2] + beliefThree[bel_3];
    return temp_string;
}

string SoulViewWindow::generateZone(PlaneType plane)
{
    float total_souls_of_plane = (float)population_manager->GetTotalSoulsForPlane(plane);

    float zone_1 = population_manager->GetZonePopulation(plane, ZoneType(1)) / total_souls_of_plane;
    float zone_2 = zone_1 + population_manager->GetZonePopulation(plane, ZoneType(2)) / total_souls_of_plane;
    float zone_3 = zone_2 + population_manager->GetZonePopulation(plane, ZoneType(3)) / total_souls_of_plane;
    float zone_4 = zone_3 + population_manager->GetZonePopulation(plane, ZoneType(4)) / total_souls_of_plane;
    float zone_5 = zone_4 + population_manager->GetZonePopulation(plane, ZoneType(5)) / total_souls_of_plane;
    float zone_6 = zone_5 + population_manager->GetZonePopulation(plane, ZoneType(6)) / total_souls_of_plane;

    int chosen_zone;
    float rand_perc = (rand() % 100) * 0.01f;
    if (zone_1 > rand_perc)
    {
        chosen_zone = 0;
    }
    else if (zone_2 > rand_perc)
    {
        chosen_zone = 1;
    }
    else if (zone_3 > rand_perc)
    {
        chosen_zone = 2;
    }
    else if (zone_4 > rand_perc)
    {
        chosen_zone = 3;
    }
    else if (zone_5 > rand_perc)
    {
        chosen_zone = 4;
    }
    else if (zone_6 > rand_perc)
    {
        chosen_zone = 5;
    }
    else
    {
        chosen_zone = 6;
    }

    if (plane == Heaven)
    {
        chosen_zone += 7;
    }

    string temp_string = available_punishments[chosen_zone];
    return temp_string;
}
