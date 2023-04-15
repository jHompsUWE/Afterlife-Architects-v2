#pragma once
#include "UIWindow.h"

class MicromanagerWindow : public UIWindow
{
public:

    MicromanagerWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice,
        std::string _text, std::string _filepath, Vector2 _setScale, std::shared_ptr<EconomyManager> _economy_manager);

    ~MicromanagerWindow() override;

    void update(GameData* _gameData, Vector2& _mousePosition) override;
    void render(DrawData2D* _drawData) override;
    const bool& ReceiveEvents(const AL::Event& al_event) override;

    void reSize(Vector2 game_res) override;

private:
    
    void updateSlider();
    
    std::shared_ptr<EconomyManager> economy_manager;

    //Images
    std::vector<ImageGO2D*> image_vec;

    Vector2 pointed_tile;
    bool tile_is_heaven;
    int slider_percent = 50; //[0 to 100] represents [100% left to 100% right]
    int prev_slider_percent = 50;
    int slider_radius = 92;
};


