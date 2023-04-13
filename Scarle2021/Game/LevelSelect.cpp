#include "pch.h"
#include "EconomyManager.h"
#include "LevelSelect.h"

LevelSelect::LevelSelect()
= default;

LevelSelect::~LevelSelect()
{
    for (auto button : buttons)
    {
        delete button;        
    }
}

bool LevelSelect::init()
{
    // level select background image
    main_menu_bg = new ImageGO2D("MainmenuBG",DataManager::GetD3DDevice());
    main_menu_bg->SetOrigin(Vector2(0,0));
    
    // UI buttons init................
    //Easy button
    buttons.push_back(new Button<AL::Game::Action, int>(Vector2(10000,10000),DataManager::GetD3DDevice(),
        "Easy","ButtonBackgroundMM", AL::EventType::event_game, AL::Game::Action::enter_gameplay,
        0,Vector2(0.5,0.5)));
    
    //Medium button
    buttons.push_back(new Button<AL::Game::Action, int>(Vector2(10000,10000),DataManager::GetD3DDevice(),
        "Medium","ButtonBackgroundMM",AL::EventType::event_game, AL::Game::Action::enter_gameplay,
        0,Vector2(0.5,0.5)));
    
    //Bard button
    buttons.push_back(new Button<AL::Game::Action, int>(Vector2(10000,10000),DataManager::GetD3DDevice(),
        "Hard","ButtonBackgroundMM",AL::EventType::event_game, AL::Game::Action::enter_gameplay,
        0,Vector2(0.5,0.5)));
    
    //Return Main menu button
    buttons.push_back(new Button<AL::Game::Action, int>(Vector2(10000,10000),DataManager::GetD3DDevice(),
        "Return to Main Menu","ButtonBackgroundMM",AL::EventType::event_game,
        AL::Game::Action::enter_main_menu, 0,Vector2(0.5,0.5)));
    
    return true;
}

void LevelSelect::Update(GameData* game_data)
{
    for (auto& button : buttons)
    {
        button->update(game_data);
    }

    main_menu_bg->Tick(game_data);
}

void LevelSelect::ScaledUpdate(GameData* game_data, float& scaled_dt)
{
}

void LevelSelect::LateUpdate(GameData* game_data)
{
}

void LevelSelect::GetEvents(const AL::Event& al_event)
{
    switch (al_event.type)
    {
    case AL::event_ui:

        switch (al_event.ui.action)
        { 
        case AL::UI::resize_ui:
            ResizeUI();
            break;
            
        default:
            break;
        }

        break;

    case AL::event_game:

        switch (al_event.game.action)
        {
         case AL::Game::enter_gameplay:
            MoveOffScreen();
            AL::NewEventManager::FlushEventListSt();
            DataManager::GetGD()->current_game_state = gs_gameplay;
            break;
            
        case AL::Game::enter_main_menu:
            MoveOffScreen();
            AL::NewEventManager::FlushEventListSt();
            DataManager::GetGD()->current_game_state = gs_main_menu;
            break;
            
        case AL::Game::quit_game:
            break;
            
        default:
            break;
        }
        
        break;
        
    default:
        break;
    }
}

void LevelSelect::Render2D(DrawData2D* draw_data2D)
{
    if(do_once)
    {
        MoveOnScreen();
        ResizeUI();
        do_once = false;
    }
    
    main_menu_bg->Draw(draw_data2D);
    
    for (auto& button : buttons)
    {
        button->render(draw_data2D);
    }
}

void LevelSelect::Render3D(DrawData* draw_data)
{
}

void LevelSelect::ResizeUI()
{
    Vector2 game_res = Vector2(*DataManager::GetRES().first, *DataManager::GetRES().second);
    main_menu_bg->ReSize(game_res.x, game_res.y);
    
    for (auto& button : buttons)
    {
        button->reSize(game_res);
        button->update(DataManager::GetGD());
    }
}

void LevelSelect::MoveOffScreen()
{
    for (auto& button : buttons)
    {
        button->setPostion(Vector2(10000,10000));
    }
    do_once = true;
}

void LevelSelect::MoveOnScreen() const
{
    buttons[0]->setPostion(Vector2(22,16));
    buttons[1]->setPostion(Vector2(449,16));
    buttons[2]->setPostion(Vector2(873,16));
    buttons[3]->setPostion(Vector2(175,640));
}
