#include "pch.h"
#include "MainMenu.h"
#include "EconomyManager.h"
#include "AStarPathfinding.h"

MainMenu::MainMenu()
= default;

MainMenu::~MainMenu()
{
    for (auto button : buttons)
    {
        delete button;        
    }
}

bool MainMenu::init()
{
    // main menu background image
    main_menu_bg = new ImageGO2D("MainmenuBG",DataManager::GetD3DDevice());
    main_menu_bg->SetOrigin(Vector2(0,0));

    // UI buttons init................
    //start button
    buttons.push_back(new Button<AL::Game::Action, int>(Vector2(10000,10000),DataManager::GetD3DDevice(),
        "Start Game","ButtonBackgroundMM", AL::EventType::event_game, AL::Game::Action::enter_level_select, 0,Vector2(0.5,0.5)));

    
    //load button
    buttons.push_back(new Button<AL::Game::Action, int>(Vector2(10000,10000),DataManager::GetD3DDevice(),
        "Load Game","ButtonBackgroundMM",AL::EventType::event_game, AL::Game::Action::enter_level_select, 0,Vector2(0.5,0.5)));
    
    //scenario button
    buttons.push_back(new Button<AL::Game::Action, int>(Vector2(10000,10000),DataManager::GetD3DDevice(),
        "Load Scenario","ButtonBackgroundMM",AL::EventType::event_game, AL::Game::Action::enter_level_select, 0,Vector2(0.5,0.5)));
    
    //intro replay button
    buttons.push_back(new Button<AL::Game::Action, int>(Vector2(10000,10000),DataManager::GetD3DDevice(),
        "Replay Intro","ButtonBackgroundMM",AL::EventType::event_game, AL::Game::Action::enter_level_select, 0,Vector2(0.5,0.5)));
    
    //Quit afterlife button
    buttons.push_back(new Button<AL::Game::Action, int>(Vector2(10000,10000),DataManager::GetD3DDevice(),
        "Quit AfterLife","ButtonBackgroundMM",AL::EventType::event_game, AL::Game::quit_game, 0,Vector2(0.5,0.5)));


    std::cout <<     buttons.back()->getButtonRes().y << std::endl;
    
    return true;
}

void MainMenu::Update(GameData* game_data)
{
    for (auto& button : buttons)
    {
        button->update(game_data);
    }
    main_menu_bg->Tick(game_data);
} 

void MainMenu::ScaledUpdate(GameData* game_data, float& scaled_dt)
{
}

void MainMenu::LateUpdate(GameData* game_data)
{
}

void MainMenu::GetEvents(const AL::Event& al_event)
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
    case AL::Game::enter_level_select:
            MoveOffScreen();
            AL::NewEventManager::FlushEventListSt();
            DataManager::GetGD()->current_game_state = gs_level_select;
            break;

        case AL::Game::enter_main_menu:
            MoveOnScreen();
            AL::NewEventManager::FlushEventListSt();
            DataManager::GetGD()->current_game_state = gs_main_menu;
            break;
            
        case AL::Game::quit_game:
            PostQuitMessage(0);
            break;
                        
        default:
            break;
        }
        break;
        
    default:
        break;;
    }
}

void MainMenu::Render2D(DrawData2D* draw_data2D)
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

void MainMenu::Render3D(DrawData* draw_data)
{
}

void MainMenu::ResizeUI() const
{
    Vector2 game_res = Vector2(*DataManager::GetRES().first, *DataManager::GetRES().second);
    main_menu_bg->ReSize(game_res.x, game_res.y);
    
    for (auto& button : buttons)
    {
        button->reSize(game_res);
    }
}

void MainMenu::MoveOffScreen()
{
    for (auto& button : buttons)
    {
        button->setPostion(Vector2(10000,10000));
    }
    do_once = true;
}

void MainMenu::MoveOnScreen() const
{
    buttons[0]->setPostion(Vector2(22,16));
    buttons[1]->setPostion(Vector2(449,16));
    buttons[2]->setPostion(Vector2(873,16));
    buttons[3]->setPostion(Vector2(175,640));
    buttons[4]->setPostion(Vector2(704,640));
}