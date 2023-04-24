#include "pch.h"
#include "GamePlay.h"

GamePlay::GamePlay()
= default;

GamePlay::~GamePlay()
{
    delete window_boarder;
    delete window_one_gate;
    delete advisor_window;
    delete window_two_karma_station;
    delete window_three_topias;
    delete window_four_training_centers_window;
    delete window_file;
    delete window_global;
    delete ui_window_event;
    delete ui_window_bad_things;
    delete ui_window_event_warning;
}

bool GamePlay::init()
{
    // Economy
    economy_manager = GameplaySingletons::GetEconomyManager();

    // Hierarchy
    hierarchy_manager = std::make_unique<HierarchyManager>();

    //window border file button
    window_file = new Window_file(Vector2(0, 35), DataManager::
        GetD3DDevice(), "", "Window_file", Vector2(1, 1));

    //window bad things button
    ui_window_bad_things = new UIWindowBadThings(Vector2(250, 35), DataManager::
        GetD3DDevice(), "", "bad_things", Vector2(1.5, 1.5));
    
    //window bad things button
    ui_window_audio_settings = new UIWindowAudioSettings(Vector2(450, 350), DataManager::
        GetD3DDevice(), "", "audio_window", Vector2(1, 1));
    hierarchy_manager->AddToHierarchy(ui_window_audio_settings);

    //window border global button
    window_global = new Window_Global(Vector2(90, 35), DataManager::
        GetD3DDevice(), "", "Global", Vector2(1.5, 1.5));

    // ui frame init
    window_boarder = new WindowBoarder(Vector2(0, 0),
        DataManager::GetD3DDevice(), "", "UIFrame", Vector2(1, 1));

    //build panel
    main_panel = new UIPanel(Vector2(0, 30), DataManager::GetD3DDevice(), "UIPanel", Vector2(1, 1));
    main_panel->setPostion(Vector2(10000, 10000));

    //advisor
    advisor_window = new AdvisorWindow(Vector2(675, 30), DataManager::
        GetD3DDevice(), "", "AdvisorBackground", Vector2(0.5, 0.5));
    hierarchy_manager->AddToHierarchy(advisor_window);

    //soul view
    soul_view = new SoulViewWindow(Vector2(400, 120), DataManager::
        GetD3DDevice(), "", "SoulView", Vector2(1, 1), GameplaySingletons::GetPopulationManager());
    hierarchy_manager->AddToHierarchy(soul_view);

    //micro manager
    macro_manager = new MacromanagerWindow(Vector2(400, 120), DataManager::
        GetD3DDevice(), "", "micro_window", Vector2(1, 1), GameplaySingletons::GetEconomyManager());
    hierarchy_manager->AddToHierarchy(macro_manager);

    //graphview
    graphview = new GraphviewWindow(Vector2(400, 120), DataManager::
        GetD3DDevice(), "", "graphview_back", Vector2(1, 1), GameplaySingletons::GetPopulationManager());
    hierarchy_manager->AddToHierarchy(graphview);

    //window gate
    window_one_gate = new UIWindow(Vector2(*DataManager::GetRES().first * 0.5, *DataManager::GetRES().second * 0.5),
        DataManager::GetD3DDevice(), "", "Window", Vector2(0.5, 0.5));
    hierarchy_manager->AddToHierarchy(window_one_gate);

    //karma station
    window_two_karma_station = new KarmaStationWindow
    (Vector2(*DataManager::GetRES().first * 0.5, *DataManager::GetRES()
        .second * 0.5), DataManager::
        GetD3DDevice(), "", "Window", Vector2(0.5, 0.5));
    hierarchy_manager->AddToHierarchy(window_two_karma_station);

    //topias
    window_three_topias = new TopiasWindowUI(Vector2(*DataManager::GetRES().first * 0.5, *DataManager::GetRES()
        .second * 0.5), DataManager::
        GetD3DDevice(), "", "Window", Vector2(0.5, 0.5));
    hierarchy_manager->AddToHierarchy(window_three_topias);

    //training center
    window_four_training_centers_window = new TrainingCentersWindow
    (Vector2(*DataManager::GetRES().first * 0.5, *DataManager::GetRES()
        .second * 0.5), DataManager::
        GetD3DDevice(), "", "Window", Vector2(0.5, 0.5));
    hierarchy_manager->AddToHierarchy(window_four_training_centers_window);

    //ingame event window
    ui_window_event = new UIWindowEvent(Vector2(*DataManager::GetRES().first * 0.5, *DataManager::GetRES()
        .second * 0.5), DataManager::GetD3DDevice(), "", "Event", Vector2(1.5, 1.5));
    hierarchy_manager->AddToHierarchy(ui_window_event);

    //window event warning button 
    ui_window_event_warning = new UIWindowEventWarning(Vector2(*DataManager::GetRES().first * 0.5, *DataManager::GetRES()
        .second * 0.5), DataManager::
        GetD3DDevice(), "", "EventWarning", Vector2(1.5, 1.5));
    hierarchy_manager->AddToHierarchy(ui_window_event_warning);

    // Mouse
    mouse_screen_pos.z = 0;
    mouse_world_pos = std::make_shared<Vector3>(0, 0, 0);

    // Building System
    building_system = std::make_unique<BuildingSystem>(mouse_world_pos, DataManager::GetD3DDevice());

    // Advisors 
    adv_man = std::make_unique<AdvisorManager>();
    adv_man->init(advisor_window);

    //window visibility
    window_boarder->setVisibility(true);
    window_file->setVisibility(false);
    window_global->setVisibility(false);
    ui_window_bad_things->setVisibility(false);
    ui_window_audio_settings->setVisibility(false);

    window_one_gate->setVisibility(false);
    window_two_karma_station->setVisibility(false);
    window_three_topias->setVisibility(false);
    window_four_training_centers_window->setVisibility(false);
    ui_window_event_warning->setVisibility(false);
    ui_window_event->setVisibility(false);

    soul_view->setVisibility(false);
    macro_manager->setVisibility(false);
    graphview->setVisibility(false);
    advisor_window->setVisibility(false);

    //Rects the window's layers
    hierarchy_manager->MoveAllToFront();
    
    do_once = true;
    return true;
}

void GamePlay::Update(GameData* game_data)
{
    if (do_once)
    {
        main_panel->setPostion(Vector2(0, 30));
        Resize();
        do_once = false;
    }

    //updates panel
    main_panel->update(game_data, mouse_pos);
    //Window file
    window_file->update(game_data, mouse_pos);
    //Bad things selector
    ui_window_bad_things->update(game_data, mouse_pos);
    //Bad things selector
    ui_window_audio_settings->update(game_data, mouse_pos);
    //Window global
    window_global->update(game_data, mouse_pos);
    //Border
    window_boarder->update(game_data, mouse_pos);
    
    //All other windows
    hierarchy_manager->Update(game_data, mouse_pos);
    
    // Advisors
    adv_man->Update(game_data);
    // Economy
    economy_manager->Tick(game_data);
    // Building System
    building_system->Tick(game_data);
}

void GamePlay::ScaledUpdate(GameData* game_data, float& scaled_dt)
{
}

void GamePlay::LateUpdate(GameData* game_data)
{
    // Update mouse position
    mouse_screen_pos.x = mouse_pos.x;
    mouse_screen_pos.y = mouse_pos.y;
}

void GamePlay::GetEvents(const AL::Event& al_event)
{
    switch (al_event.type)
    {
    case AL::event_input:
        break;

    case AL::event_cursor_move:
        mouse_pos.x = (float)al_event.cursor_moved.pos_x;
        mouse_pos.y = (float)al_event.cursor_moved.pos_y;
        break;

    case AL::event_cursor_interact:
        break;

    case AL::event_sound_start:
        break;

    case AL::event_sound_stop:
        break;

    case AL::event_ui:

        switch (al_event.ui.action)
        {
        case AL::UI::window_gate:
            hierarchy_manager->OpenCloseWindow(window_one_gate);
            break;

        case AL::UI::window_karma_station:
            hierarchy_manager->OpenCloseWindow(window_two_karma_station);
            break;

        case AL::UI::window_advisors:
            hierarchy_manager->OpenCloseWindow(advisor_window);
            break;

        case AL::UI::window_topias:
            hierarchy_manager->OpenCloseWindow(window_three_topias);
            break;

        case AL::UI::window_training_centre:
            hierarchy_manager->OpenCloseWindow(window_four_training_centers_window);
            break;

        case AL::UI::window_soulview:
            soul_view->generateRandSoul();
            hierarchy_manager->OpenCloseWindow(soul_view);
            break;

        case AL::UI::window_macro_manager:
            hierarchy_manager->OpenCloseWindow(macro_manager);
            break;

        case AL::UI::window_graphview:
            hierarchy_manager->OpenCloseWindow(graphview);
            break;

        case AL::UI::window_file:
            window_file->setVisibility(!window_file->getVisibility());
            break;

        case AL::UI::window_global:
            window_global->setVisibility(!window_global->getVisibility());
            break;
        case AL::UI::global_bad_things_window:
            ui_window_bad_things->setVisibility(!ui_window_bad_things->getVisibility());
            break;
        case AL::UI::global_audio_settings_window:
            hierarchy_manager->OpenCloseWindow(ui_window_audio_settings);
            break;

        case AL::UI::no_bad_things:
            hierarchy_manager->OpenCloseWindow(ui_window_event_warning);
            ui_window_event_warning->setString("OMG such a cheat");
            break;

        case AL::UI::birds_of_paradise:
            hierarchy_manager->OpenCloseWindow(ui_window_event_warning);
            ui_window_event_warning->setString("birds are pooping every where");
            break;
        case AL::UI::bats_out_of_hell:
            hierarchy_manager->OpenCloseWindow(ui_window_event_warning);
            ui_window_event_warning->setString("rats are biting everyone");
            break;

        case AL::UI::my_blue_heaven:
            hierarchy_manager->OpenCloseWindow(ui_window_event_warning);
            ui_window_event_warning->setString("me soo sad !!");
            break;

        case AL::UI::hell_freezes_over:
            hierarchy_manager->OpenCloseWindow(ui_window_event_warning);
            ui_window_event_warning->setString("dam that is cold");
            break;

        case AL::UI::heaven_nose:
            hierarchy_manager->OpenCloseWindow(ui_window_event_warning);
            ui_window_event_warning->setString("that is a big nose");
            break;

        case AL::UI::hell_in_a_hand_basket:
            hierarchy_manager->OpenCloseWindow(ui_window_event_warning);
            ui_window_event_warning->setString("where did my sandwich go");
            break;

        case AL::UI::paradise_pair_of_dice:
            hierarchy_manager->OpenCloseWindow(ui_window_event_warning);
            ui_window_event_warning->setString("Snake eyes, you lose ");
            break;

        case AL::UI::disco_inferno:
            hierarchy_manager->OpenCloseWindow(ui_window_event_warning);
            ui_window_event_warning->setString("night fever, night fever");
            break;

        case AL::UI::file_exit_game:
            PostQuitMessage(0);
            break;

        case AL::UI::file_new_game:
            main_panel->setPostion(Vector2(1000, 30));

            //Resets window states
            window_boarder->setVisibility(true);
            window_file->setVisibility(false);
            window_global->setVisibility(false);
            ui_window_bad_things->setVisibility(false);
            ui_window_audio_settings->setVisibility(false);

            window_one_gate->setVisibility(false);
            window_two_karma_station->setVisibility(false);
            window_three_topias->setVisibility(false);
            window_four_training_centers_window->setVisibility(false);
            ui_window_event_warning->setVisibility(false);
            ui_window_event->setVisibility(false);

            soul_view->setVisibility(false);
            macro_manager->setVisibility(false);
            graphview->setVisibility(false);
            advisor_window->setVisibility(false);

            DataManager::GetGD()->current_game_state = gs_main_menu;
            AL::NewEventManager::GenerateEventSt(AL::EventType::event_game, AL::Game::Action::reset);
            break;

        default:
            break;
        }
        break;

    case AL::event_build_sys:
        break;

    case AL::event_game:
        break;

    default:
        break;
    }
}

void GamePlay::Render2D(DrawData2D* draw_data2D)
{
    hierarchy_manager->Render(draw_data2D);

    main_panel->render(draw_data2D);
    window_file->render(draw_data2D);
    ui_window_bad_things->render(draw_data2D);
    window_global->render(draw_data2D);
    window_boarder->render(draw_data2D);
}

void GamePlay::Render3D(DrawData* draw_data)
{
    UpdateMousePos(draw_data);

    // Building System
    building_system->Render3D(draw_data);
}

void GamePlay::Reset()
{
    do_once = true;
}

void GamePlay::Resize()
{
    //resize UI based on window resolution 
    screen_size = Vector2(*DataManager::GetRES().first, *DataManager::GetRES().second);
    
    window_boarder->reSize(screen_size);
    window_file->reSize(screen_size);
    window_global->reSize(screen_size);
    ui_window_bad_things->reSize(screen_size);
    main_panel->reSize(screen_size);
    hierarchy_manager->ResizeAll();
}

void GamePlay::UpdateMousePos(DrawData* draw_data)
{
    // Translate screen pos to world pos
    // TODO: update the screen width and height
    Vector3 temp_world_pos = XMVector3Unproject(mouse_screen_pos,
        0, 0, screen_size.x, screen_size.y,
        draw_data->main_camera->GetNearZ(),
        draw_data->main_camera->GetFarZ(),
        draw_data->main_camera->GetProj(),
        draw_data->main_camera->GetView(),
        draw_data->main_camera->GetWorldMatrix());

    *mouse_world_pos = Raycast::GetPosOnY(0, draw_data->main_camera->GetDirection(), temp_world_pos);

    // Offset by camera movement
    mouse_world_pos->x += draw_data->main_camera->GetTarget().x;
    mouse_world_pos->z += draw_data->main_camera->GetTarget().z;

    // Floor mouse_world_pos to tilemap grid (each tile is 1x1 unit)
    *mouse_world_pos = Vector3(std::floor(mouse_world_pos->x), 0, std::floor(mouse_world_pos->z));
}

