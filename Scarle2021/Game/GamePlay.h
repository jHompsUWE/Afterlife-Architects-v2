#pragma once
#include "CMOGO.h"
#include "StateTemplate.h"
#include "AdvisorManager.h"
#include "TextGO2D.h"
#include "GPGO.h"

//UI systems
#include "UIPanel.h"
#include "UIWindow.h"
#include "SoulViewWindow.h"
#include "MacromanagerWindow.h"
#include "KarmaStationWindow.h"
#include "TopiasWindowUI.h"
#include "UIWindowEvent.h"
#include "WindowBoarder.h"

#include "GraphviewWindow.h"
// Economy and Population
#include "GameplaySingletons.h"

// Building System
#include "BuildingSystem.h"
#include "HierarchyManager.h"
#include "Raycast.h"
#include "TopiasWindowUI.h"
#include "TrainingCentersWindow.h"
#include "UIWindowBadThings.h"
#include "UIWindowAudioSettings.h"
#include "UIWindowEventWarning.h"
#include "Window_file.h"
#include "Window_Global.h"

class GPGO;

class GamePlay : public StateTemplate
{
public:
    GamePlay();
    ~GamePlay() override;

    bool init() override;

    //Update Cycles
    void Update(GameData* game_data) override;
    void ScaledUpdate(GameData* game_data, float& scaled_dt) override;
    void LateUpdate(GameData* game_data) override;

    //Events
    void GetEvents(const AL::Event& al_event) override;

    //Renders
    void Render2D(DrawData2D* draw_data2D) override;
    void Render3D(DrawData* draw_data) override;

    //Reset
    void Reset() override;
    void Resize() override;

private:
    void UpdateMousePos(DrawData* draw_data);
    
    bool do_once;
    
    // Building System
    std::unique_ptr<BuildingSystem> building_system;
    
    Vector2 screen_size;
    Vector2 mouse_pos{ 0,0 };
    Vector3 mouse_screen_pos;
    std::shared_ptr<Vector3> mouse_world_pos;

    //main build panel
    UIPanel* main_panel;

    /// UI windows -------------

    //File Bar
    WindowBoarder* window_boarder;
    Window_file* window_file;
    Window_Global* window_global;
    UIWindowBadThings* ui_window_bad_things;

    // Pop up
    UIWindow* window_one_gate;
    KarmaStationWindow* window_two_karma_station;
    TopiasWindowUI* window_three_topias;
    TrainingCentersWindow* window_four_training_centers_window;
    UIWindowEventWarning* ui_window_event_warning;
    UIWindowAudioSettings* ui_window_audio_settings;
    UIWindowEvent* ui_window_event;

    // Pop up info
    SoulViewWindow* soul_view;
    MacromanagerWindow* macro_manager;
    GraphviewWindow* graphview;
    AdvisorWindow* advisor_window;

    // Adv manager
    std::unique_ptr<AdvisorManager> adv_man = nullptr;
    // Economy
    std::shared_ptr<EconomyManager> economy_manager = nullptr;
    // Window Hierarchy
    std::unique_ptr<HierarchyManager> hierarchy_manager = nullptr;
};

