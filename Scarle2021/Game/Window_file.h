#pragma once
#include "UIWindow.h"

class Window_file : public UIWindow
{
public:
    Window_file(Vector2 _windowPosition, ID3D11Device* _d3dDevice,
        std::string _text,std::string _filepath,Vector2 _setScale);
    
    ~Window_file() override;

    void update(GameData* _gameData, Vector2& _mousePosition) override;
    void render(DrawData2D* _drawData) override;
};

