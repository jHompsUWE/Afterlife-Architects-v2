#pragma once
#include "UIWindow.h"
#include "PopulationManager.h"

enum GraphShown {
    Belief, Population, Religion, No_Graph
};

class GraphviewWindow : public UIWindow
{
public:

    GraphviewWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice,
        std::string _text, std::string _filepath, Vector2 _setScale, std::shared_ptr<PopulationManager> pop_manager);

    ~GraphviewWindow() override;

    void update(GameData* _gameData, Vector2& _mousePosition) override;
    void render(DrawData2D* _drawData) override;
    const bool& ReceiveEvents(const AL::Event& al_event) override;
    
    void reSize(Vector2 game_res) override;

private:
    
    void updateBeliefSpread();
    void updateBeliefVisual();
    void updatePopulationVisual();
    void updateReligionVisual();
    
    std::shared_ptr<PopulationManager> population_manager;
    
    // Images 
    std::vector<ImageGO2D*> image_vec;
    std::vector<ImageGO2D*> belief_bars_vec;
    std::vector<ImageGO2D*> population_bars_vec;
    std::vector<ImageGO2D*> religion_bars_vec;
    float religion_bars_x[8] = { 53.5, 67.5, 81.5, 95.5, 158.5, 172.5, 250.5, 264.5 };
    Vector2 base_resolution = Vector2(352, 316);
    Vector2 max_belief_scales = Vector2(8.4, 0.2);
    Vector2 max_population_scales = Vector2(4.2, 0.275);
    Vector2 max_religion_scales = Vector2(0.25, 1.925);

    GraphShown cur_graph = No_Graph;
    float timer = 0;
    float time_between_updates = 2;
    
    float belief_amounts[16] = {0.5,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    float rel_spreads[8] = {0,0,0,0,0,0,0,0};
};

