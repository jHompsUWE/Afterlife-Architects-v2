#include "pch.h"
#include "UIPanel.h"

UIPanel::UIPanel(Vector2 _panelPosition, ID3D11Device*
                 _d3dDevice, std::string _filepath,Vector2 _newScale)
{
    //setup for button background
    panel_back_ground = new ImageGO2D(_filepath, _d3dDevice);
    panel_back_ground->SetOrigin(Vector2(0, 0));
    panel_back_ground->SetScale(Vector2(0.8,0.7));
    
    //sets res
    panel_res = Vector2(panel_back_ground->GetRes().x
        * panel_back_ground->GetScale().x, panel_back_ground->GetRes().y
        * panel_back_ground->GetScale().y);
	
    panel_pos = _panelPosition;
    panel_back_ground->SetPos(panel_pos);

    population_manager = GameplaySingletons::GetPopulationManager();
    economy_manager = GameplaySingletons::GetEconomyManager();
    
    // UI game play buttons
    
    //row 1...................
    buttons.push_back(new Button<AL::BuildSys::Section, ZoneType>(Vector2(panel_pos.x + 51,panel_pos.y + 84),DataManager::GetD3DDevice()
        ,"green",AL::EventType::event_build_sys, AL::BuildSys::Section::zone, Green ,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::BuildSys::Section, ZoneType>(Vector2(panel_pos.x + 94,panel_pos.y + 84),DataManager::GetD3DDevice()
        ,"green",AL::EventType::event_build_sys, AL::BuildSys::Section::zone, Yellow,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::BuildSys::Section, ZoneType>(Vector2(panel_pos.x + 138,panel_pos.y + 84),DataManager::GetD3DDevice()
        ,"green",AL::EventType::event_build_sys, AL::BuildSys::Section::zone, Orange,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::BuildSys::Section, ZoneType>(Vector2(panel_pos.x + 180,panel_pos.y + 84),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_build_sys, AL::BuildSys::Section::zone, Brown,Vector2(0.8,0.7)));
    
    //row 2......................
    buttons.push_back(new Button<AL::BuildSys::Section, ZoneType>(Vector2(panel_pos.x + 50,panel_pos.y + 113),DataManager::GetD3DDevice()
            ,"green",AL::EventType::event_build_sys, AL::BuildSys::Section::zone, Purple,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::BuildSys::Section, ZoneType>(Vector2(panel_pos.x + 93,panel_pos.y + 113),DataManager::GetD3DDevice()
        ,"green",AL::EventType::event_build_sys, AL::BuildSys::Section::zone, Red,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::BuildSys::Section, ZoneType>(Vector2(panel_pos.x + 137,panel_pos.y + 113),DataManager::GetD3DDevice()
        ,"green",AL::EventType::event_build_sys, AL::BuildSys::Section::zone, Blue,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::BuildSys::Section, ZoneType>(Vector2(panel_pos.x + 180,panel_pos.y + 113),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_build_sys, AL::BuildSys::Section::zone, Void,Vector2(0.8,0.7)));
    
    //row 3 ..................
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 50,panel_pos.y + 143),DataManager::GetD3DDevice()
            ,"green", AL::EventType::event_ui, AL::UI::window_gate, 0,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::BuildSys::Section, ZoneType>(Vector2(panel_pos.x + 93,panel_pos.y + 143),DataManager::GetD3DDevice()
        ,"green",AL::EventType::event_build_sys, AL::BuildSys::Section::zone, Road,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 137,panel_pos.y + 143),DataManager::GetD3DDevice()
        ,"green", AL::EventType::event_ui, AL::UI::window_karma_station, 0,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::BuildSys::Section, ZoneType>(Vector2(panel_pos.x + 180,panel_pos.y + 143),DataManager::GetD3DDevice()
        ,"green",AL::EventType::event_build_sys, AL::BuildSys::Section::zone, Karma_Tracks,Vector2(0.8,0.7)));
    
    // //row 4 ..........................
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 50,panel_pos.y + 173),DataManager::GetD3DDevice()
        ,"green",AL::EventType::event_ui, AL::UI::window_topias, 0,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 93,panel_pos.y + 173),DataManager::GetD3DDevice()
        ,"green",AL::EventType::event_ui, AL::UI::window_training_centre, 0,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 137,panel_pos.y + 173),DataManager::GetD3DDevice()
        ,"green",AL::EventType::event_ui, AL::UI::window_ports, 0,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 180,panel_pos.y + 173),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_ui, AL::UI::window_siphon, 0,Vector2(0.8,0.7)));
    
    // row 5...............................
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 50,panel_pos.y + 203),DataManager::GetD3DDevice()
        ,"green",AL::EventType::event_ui, AL::UI::window_special_buildings, 0,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 94,panel_pos.y + 203),DataManager::GetD3DDevice()
        ,"green",AL::EventType::event_ui, AL::UI::window_omnibolges_love_domes, 0,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 137,panel_pos.y + 203),DataManager::GetD3DDevice()
        ,"green",AL::EventType::event_ui, AL::UI::window_limbo_structures, 0,Vector2(0.8,0.7)));
    
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 180,panel_pos.y + 203),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_ui, AL::UI::window_delete_structures, 0,Vector2(0.8, 0.7)));
    
    // //zoom button 
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 155,panel_pos.y + 235),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_ui, AL::UI::button_zoom_in, 0,Vector2(0.8, 0.7)));
        
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 180,panel_pos.y + 280),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_ui, AL::UI::button_zoom_out, 0,Vector2(0.8, 0.7)));
    
    //direction buttons     
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 80,panel_pos.y + 235),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_ui, AL::UI::window_delete_structures, 0,Vector2(0.8, 0.7)));
        
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 180,panel_pos.y + 203),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_ui, AL::UI::window_delete_structures, 0,Vector2(0.8, 0.7)));
        
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 180,panel_pos.y + 203),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_ui, AL::UI::window_delete_structures, 0,Vector2(0.8, 0.7)));
        
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 180,panel_pos.y + 203),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_ui, AL::UI::window_delete_structures, 0,Vector2(0.8, 0.7)));
        
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 180,panel_pos.y + 203),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_ui, AL::UI::window_delete_structures, 0,Vector2(0.8, 0.7)));
    
    //advisor button
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 83,panel_pos.y + 355),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_ui, AL::UI::window_advisors, 0,Vector2(2.8, 1.5)));

    //graphview button
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 51,panel_pos.y + 310),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_ui, AL::UI::window_graphview, 0,Vector2(0.8, 0.7)));
    
    //soul view button
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 93,panel_pos.y + 310),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_ui, AL::UI::window_soulview, 0,Vector2(0.8, 0.7)));

    //micro manager button
    buttons.push_back(new Button<AL::UI::Action, int>(Vector2(panel_pos.x + 135,panel_pos.y + 311),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_ui, AL::UI::window_micro_manager, 0,Vector2(0.8, 0.7)));

    buttons.push_back(new Button<AL::Input::Action, int>(Vector2(panel_pos.x + 180,panel_pos.y + 311),DataManager::GetD3DDevice()
    ,"green",AL::EventType::event_input, AL::Input::show_vibes, 0,Vector2(0.8, 0.7)));
    //.............
    
    //UI Text Vector

    text.push_back(new TextGO2D("Year: " + std::to_string(economy_manager->GetYear())));
    text[0]->SetPos(Vector2(30,35));
    text[0]->SetScale(Vector2(0.5,0.5));
    text[0]->SetColour(Color((float*)&Colors::Green));

    text.push_back(new TextGO2D("Credits: " + std::to_string(economy_manager->GetMoney())));
    text[1]->SetPos(Vector2(30,60));
    text[1]->SetScale(Vector2(0.5,0.5));
    text[1]->SetColour(Color((float*)&Colors::Green));

    text.push_back(new TextGO2D(" " + std::to_string(population_manager->GetTotalSouls())));
    text[2]->SetPos(Vector2(135,455));
    text[2]->SetScale(Vector2(0.4,0.4));
    text[2]->SetColour(Color((float*)&Colors::Green));

    setPostion(Vector2(1000, 30));
    
    //Subscribe for events
    AL::NewEventManager::AddEventReceiver(this, AL::EventType::event_cursor_interact);
}

UIPanel::~UIPanel()
{
    for (auto button : buttons)
    {
        delete button;        
    }

    // updates texts
    for (auto& text : text)
    {
        delete text;
    }
    delete panel_back_ground;

    //Unsubscribe from events
    AL::NewEventManager::RemoveEventReceiver(this);
}

void UIPanel::update(GameData* _gameData, Vector2& _mousePosition)
{
    mouse_pos = _mousePosition;
    
    panel_back_ground->Tick(_gameData);
    //updates buttons

    if (economy_manager->GetMoney() < 10000)
    {
        text[1]->ChangeString("Credit: " + std::to_string((int)economy_manager->GetMoney()));
    }
    
    else if (economy_manager->GetMoney() < 1000000)
    {
        text[1]->ChangeString("Credit: " + (std::to_string((int)economy_manager->GetMoney()/1000) + " K"));
    }

    else if (economy_manager->GetMoney() > 1000000)
    {
        text[1]->ChangeString("Credit: " + (std::to_string((int)economy_manager->GetMoney()/1000000) + " M"));
    }

    //converts float to string to int
    text[0]->ChangeString("Year: " +std::to_string((int)economy_manager->GetYear()));
    text[2]->ChangeString(std::to_string((int)population_manager->GetTotalSouls()));

    for (auto& button : buttons)
    {
        button->update(_gameData);
    }

    // updates texts
    for (auto& text : text)
    {
        text->Tick(_gameData);
    }

    inside = isInsidePanel(mouse_pos);
    
    //if clicked updates pos and scale for window drag  
    if (toggle_click_panel && inside)
    {

        //new pos on click and drag 
        Vector2 offset = old_mouse_pos_panel - mouse_pos;
        panel_back_ground->SetPos(panel_back_ground->GetPos()-offset);
        panel_pos = panel_back_ground->GetPos();
        
        //button pos
        for (auto& button : buttons)
        {
            Vector2 const panel_pos = button->getPosition();
            button->setPostion(panel_pos - offset);
            
        }
        
        // updates texts
        for (auto& text : text)
        {
            Vector2 const panel_pos = text->GetPos();
            text->SetPos(panel_pos - offset);
        }
    }
    old_mouse_pos_panel = mouse_pos;
}

void UIPanel::render(DrawData2D* _drawData)
{
    //renders buttons
    for (auto& button : buttons)
    {
        button->render(_drawData);
    }  
   
    panel_back_ground->Draw(_drawData);
    // updates texts
    for (auto& text : text)
    {
        text->Draw(_drawData);
    }
}

const bool& UIPanel::ReceiveEvents(const AL::Event& al_event)
{
    if(al_event.cursor_interact.action == AL::Cursor::button_input1)
    {
        toggle_click_panel = al_event.cursor_interact.active;
    }
    return false;
}

const bool& UIPanel::IsCursorInsideWindow()
{
    return inside;
}

void UIPanel::setPostion(Vector2 _panelPosition)
{
    if(_panelPosition == panel_pos) return;
    
    //Updates the position of the panel with a 
    const Vector2 offset = _panelPosition - panel_pos;
    panel_back_ground->SetPos(_panelPosition);

    for (auto& button : buttons)
    {
        button->setPostion(button->getPosition() + offset);
    }

    for(auto& t : text)
    {
        t->SetPos(t->GetPos() + offset);
    }
    
    panel_pos = _panelPosition;
}

void UIPanel::setScale(Vector2& _newScale)
{
}

Vector2& UIPanel::getPosition()
{
    return panel_pos;
}

Vector2& UIPanel::getButtonRes()
{
    return panel_res;
}

void UIPanel::reSize(Vector2 game_res)
{
    auto& scale = panel_back_ground->ReSize
    (game_res.x, game_res.y);
    panel_pos = panel_pos * scale;
    panel_res = panel_res * scale;
    
    for (auto& button : buttons)
    {
        button->reSize(game_res);
    }

    // updates texts
    for (auto& text : text)
    {
        text->ReSize(game_res.x,game_res.y);
    }
}

bool UIPanel::isInsidePanel(Vector2& point) const
{
    if(point.x >= panel_pos.x && point.x <= (panel_pos.x + panel_res.x) &&
       point.y >= panel_pos.y && point.y <= (panel_pos.y + panel_res.y))
           return true;
    
    return false;
}
