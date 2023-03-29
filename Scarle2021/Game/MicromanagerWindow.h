#pragma once
#include "UIWindow.h"

class MicromanagerWindow : public IEventReceiver
{
public:

    MicromanagerWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice,
        std::string _text, std::string _filepath, Vector2 _setScale);

    ~MicromanagerWindow() override;

    void update(GameData* _gameData, Vector2& _mousePosition);
    void render(DrawData2D* _drawData);
    void ReceiveEvents(const AL::Event& al_event) override;

    void set_postion(Vector2& _new_pos);
    void set_scale(Vector2& _newScale);

    Vector2& getPosition();
    Vector2& getButtonRes();

    void reSize(Vector2 game_res);
    bool is_visible = false;

private:
    void updateSlider();
    //mouse pointer inside window
    bool isInside(Vector2& point) const;
    bool toggle_click = false;
    Vector2 mouse_pos = { 0,0 };
    Vector2 old_mouse_pos{ 0,0 };

    ImageGO2D* windowBackGround = nullptr;

    //vector of buttons 
    std::vector<UIButtonInterFace*> buttons;
    std::vector<TextGO2D*> text_vec;
    std::vector<ImageGO2D*> image_vec;

    Vector2 pointed_tile;
    bool tile_is_heaven;
    int slider_percent = 50; //[0 to 100] represents [100% left to 100% right]
    int prev_slider_percent = 50;
    int slider_radius = 92;

    //vectors
    Vector2 window_res{ 0,0 };
    Vector2 window_pos{ 0,0 };
};


