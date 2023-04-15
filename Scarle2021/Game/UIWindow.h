#pragma once
#include "Button.hpp"
#include "GameData.h"
#include "ImageGO2D.h"
#include "TextGO2D.h"
#include "BuildingManager.h"

class UIWindow : public AL::EventReceiver
{
public:
    UIWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice,
        std::string _text,std::string _filepath,Vector2 _setScale);
    
    UIWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice,
        std::string _filepath,Vector2 _setScale);
    UIWindow();

    ~UIWindow() override;

    virtual void update(GameData* _gameData, Vector2& _mousePosition);
    virtual void render(DrawData2D* _drawData);

    //Overriden From Event Manager
    const bool& ReceiveEvents(const AL::Event& al_event) override;
    const bool& IsCursorInsideWindow() override;
    
    virtual void setPostion(Vector2& _new_pos);
    virtual void setScale(Vector2& _newScale);

    virtual Vector2& getPosition();
    virtual Vector2& getButtonRes();

    virtual void reSize(Vector2 game_res);
    virtual Vector2 getWindowRes();

    virtual void setVisibility(bool _vis);
    virtual const bool& getVisibility();

    virtual void MoveInFront();
    virtual void MoveToBack();

    
protected:
    //mouse pointer inside window
    virtual bool isInside(Vector2& point) const;
    
    bool toggle_click = false;
    Vector2 mouse_pos {0,0};
    Vector2 old_mouse_pos{0,0};
    
    ImageGO2D* windowBackGround = nullptr;
  
    //vector of buttons 
    std::vector<UIButtonInterFace*> buttons;
    std::vector<TextGO2D*> text_vec;

    //vectors
    Vector2 window_res {0,0};
    Vector2 window_pos {0,0};

    //enum for texts pos
    enum texts_enum
    {
        text1, text2, text3, text4,
    };
    // makes window render or update 
    bool is_visible = false;
    bool inside = false;
};

