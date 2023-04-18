#pragma once
#include "UIWindow.h"
#include "UISlider.h"

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
    
    void updateSlider(int perc_change);
    
    std::shared_ptr<EconomyManager> economy_manager;

    //Images
    std::vector<ImageGO2D*> image_vec;
    UISlider* rad_slider;
    Vector2 pointed_tile;
    bool tile_is_heaven;
};


