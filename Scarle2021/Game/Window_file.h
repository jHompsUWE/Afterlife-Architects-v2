#pragma once
#include "UIWindow.h"

class Window_file :
    public UIWindow
{
public:
    Window_file(Vector2 _windowPosition, ID3D11Device* _d3dDevice,
        std::string _text,std::string _filepath,Vector2 _setScale);
    
    ~Window_file();

    void update(GameData* _gameData, Vector2& _mousePosition);
    void render(DrawData2D* _drawData);

    void set_postion(Vector2& _new_pos) ;
    void setScale(Vector2& _newScale) ;
	
    Vector2& getPosition() ;
    Vector2& getButtonRes() ;
	
    void reSize(Vector2 game_res);

    Vector2 getWindowRes() { return window_res; }
};

