#include "pch.h"
#include "GamePlay.h"

GamePlay::GamePlay()
= default;

GamePlay::~GamePlay()
{
    delete tilemap;
}

bool GamePlay::init()
{
    text = new TextGO2D("this ist GAMEPLAYYY");
    text->SetPos(Vector2(100, 10));
    text->SetColour(Color((float*)&Colors::Yellow));

    tilemap = new Tilemap(DataManager::GetD3DDevice(), 50);

    preview_quad = new PreviewQuad(DataManager::GetD3DDevice());
    preview_quad->SetPos(Vector3(0, 0.01f, 0));
    show_preview_quad = false;

    mouse_screen_pos.z = 0;
    mouse_world_pos.y = 0;

    selected_zone = Green;
    
    return true;
}

void GamePlay::Update(GameData* game_data)
{
    if (do_once)
    {
        do_once = false;
        // Tick tilemap once to update position
        tilemap->Tick(game_data);
    }

    preview_quad->Tick(game_data);
}

void GamePlay::ScaledUpdate(GameData* game_data, float& scaled_dt)
{
}

void GamePlay::LateUpdate(GameData* game_data)
{
    // Update mouse position
    mouse_screen_pos.x = game_data->mouse_state.x;
    mouse_screen_pos.y = game_data->mouse_state.y;

    if (game_data->mouse_state.leftButton)
    {
        if (!mouse_pressed)
        {
            // Mouse Pressed
            mouse_pressed_pos = mouse_world_pos;
            mouse_pressed = true;
        }

        // Mouse Hold
        preview_quad->ResizePreviewQuad(mouse_pressed_pos, mouse_world_pos);
    }
    else
    {
        if (mouse_pressed)
        {
            // Mouse Released
            mouse_released_pos = mouse_world_pos;
            mouse_pressed = false;

            preview_quad->ResetPreviewQuad();
            tilemap->BoxFill(selected_zone, mouse_pressed_pos, mouse_released_pos);
        }
    }
}

void GamePlay::GetEvents(std::list<AfterlifeEvent>& event_list)
{
    for (auto& event : event_list)
    {
        switch (event)
        {
        case number_1:
            selected_zone = Green;
            preview_quad->StartPreviewQuad(selected_zone);
            break;

        case number_2:
            selected_zone = Yellow;
            preview_quad->StartPreviewQuad(selected_zone);
            break;

        case number_3:
            selected_zone = Orange;
            preview_quad->StartPreviewQuad(selected_zone);
            break;

        case number_4:
            selected_zone = Brown;
            preview_quad->StartPreviewQuad(selected_zone);
            break;

        case number_5:
            selected_zone = Purple;
            preview_quad->StartPreviewQuad(selected_zone);
            break;

        case number_6:
            selected_zone = Red;
            preview_quad->StartPreviewQuad(selected_zone);
            break;

        case number_7:
            selected_zone = Blue;
            preview_quad->StartPreviewQuad(selected_zone);
            break;

        case number_8:
            selected_zone = Void;
            preview_quad->StartPreviewQuad(selected_zone);
        }
    }
}

void GamePlay::Render2D(DrawData2D* draw_data2D)
{
    text->Draw(draw_data2D);
}

void GamePlay::Render3D(DrawData* draw_data)
{
    tilemap->Draw(draw_data);
    
    if (mouse_pressed)
    {
        preview_quad->Draw(draw_data);
    }

    // Translate screen pos to world pos
    // TODO: update the screen width and height
    Vector3 temp_world_pos = XMVector3Unproject(mouse_screen_pos, 0, 0, 1080, 720,
        draw_data->main_camera->GetNearZ(),
        draw_data->main_camera->GetFarZ(),
        draw_data->main_camera->GetProj(),
        draw_data->main_camera->GetView(), 
        draw_data->main_camera->GetWorldMatrix());

    mouse_world_pos = Raycast::GetPosOnY(0, draw_data->main_camera->GetDirection(), temp_world_pos);

    // Offset by camera movement
    mouse_world_pos.x += draw_data->main_camera->GetTarget().x;
    mouse_world_pos.z += draw_data->main_camera->GetTarget().z;

    // Floor mouse_world_pos to tilemap grid (each tile is 1x1 unit)
    mouse_world_pos = Vector3(std::floor(mouse_world_pos.x), 0, std::floor(mouse_world_pos.z));
}


