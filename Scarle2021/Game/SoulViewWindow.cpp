#include "pch.h"
#include "SoulViewWindow.h"

#include "DataManager.h"

SoulViewWindow::SoulViewWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice, std::string _text,
    std::string _filepath, Vector2 _setScale, std::shared_ptr<PopulationManager> pop_manager)
    : UIWindow(), population_manager(pop_manager)
{
    srand(std::time(nullptr));
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

/// <summary>
/// Randomise the text shown for name, reward/punishment, time left on the plane, and religion of a soul in the aferlife
/// </summary>
void SoulViewWindow::generateRandSoul()
{
    // Don't generate soul if there are none
    if (population_manager->GetTotalSouls() == 0) { return; }

    // Get random name out of name array
    string temp_string = random_names[std::rand() % random_names.size()];
    text_vec[0]->ChangeString(temp_string);

    float heaven_percentage = population_manager->GetTotalSoulsForPlane(Heaven) / (float)population_manager->GetTotalSouls();
    PlaneType hell_or_heaven;
    // If distribution is ONLY hell
    if (heaven_percentage == 0)
    {
        hell_or_heaven = Hell;
    }
    // If distribution is ONLY heaven
    else if (heaven_percentage == 100)
    {
        hell_or_heaven = Heaven;
    }
    else
    {
        // Otherwise get random chance of either plane
        float rand_perc = (rand() % 100) * 0.01f;
        if (heaven_percentage > rand_perc) { hell_or_heaven = Heaven; }
        else { hell_or_heaven = Hell; }
    }

    // Say random punishment or reward based off chosen plane
    switch (hell_or_heaven)
    {
    case Heaven: text_vec[1]->ChangeString("Rewarded for " + generateZone(hell_or_heaven)); break;
    case Hell: text_vec[1]->ChangeString("Punished for " + generateZone(hell_or_heaven)); break;
    }
    // Generate random duration of stay
    text_vec[2]->ChangeString("Years left at location: " + std::to_string(rand() % 950 + 50));
    // Generate random beleif out of percentage chances
    text_vec[3]->ChangeString("Believes in " + generateBelief(hell_or_heaven) + "ism");
}

/// <summary>
/// Generate random religion based off current population
/// </summary>
/// <param name="plane">Plane of choice Heaven or Hell</param>
/// <returns> String of decided complete religion </returns>
string SoulViewWindow::generateBelief(PlaneType plane)
{
    float total_souls_of_plane = population_manager->GetTotalSoulsForPlane(plane);

    float HAHA = population_manager->GetReligiousSpread(plane, 0) / total_souls_of_plane;
    float HOHO = population_manager->GetReligiousSpread(plane, 1) / total_souls_of_plane;
    float ALF = population_manager->GetReligiousSpread(plane, 4) / total_souls_of_plane;
    float SUMA = population_manager->GetReligiousSpread(plane, 6) / total_souls_of_plane;

    string temp_string;
    int temp_int;
    float rand_perc;

    // Generate first random belief
    rand_perc = (rand() % 100) * 0.01f;
    if (HAHA > rand_perc) { temp_int = 0; }
    else if (HAHA + HOHO > rand_perc) { temp_int = 1; }
    else
    {
        temp_int = plane == Heaven ? 3 : 2;
    }
    temp_string.append(beliefOne[temp_int]);

    // Generate second random belief
    rand_perc = (rand() % 100) * 0.01f;
    temp_int = ALF > rand_perc ? 0 : 1;
    temp_string.append(beliefTwo[temp_int]);

    // Generate third random belief
    rand_perc = (rand() % 100) * 0.01f;
    temp_int = SUMA > rand_perc ? 0 : 1;
    temp_string.append(beliefThree[temp_int]);

    return temp_string;
}

/// <summary>
/// Generate random zone based off current population
/// </summary>
/// <param name="plane"></param>
/// <returns></returns>
string SoulViewWindow::generateZone(PlaneType plane)
{
    float total_souls_of_plane = (float)population_manager->GetTotalSoulsForPlane(plane);

    // Generate random number between 0-99
    float rand_perc = (rand() % 100) * 0.01f;
    int chosen_zone;
    float chance_track = 0;
    // Iterate through each zone chance
    for (int i = 0; i < 7; i++)
    {
        // Stack onto existing chance tracker
        chance_track += population_manager->GetZonePopulation(plane, ZoneType(i+1)) / total_souls_of_plane;
        // If above chance generate then select as chosen zone
        if (chance_track > rand_perc)
        {
            // But add 7 if it is heaven to offset where it checks in the array of zone names
            chosen_zone = plane == Heaven ? i+7 : i;
            return available_punishments[chosen_zone];
        }
    }
    return "ERROR";
}
