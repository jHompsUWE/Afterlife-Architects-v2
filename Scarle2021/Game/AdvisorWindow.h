#pragma once
#include "UIWindow.h"

class AdvisorWindow : public UIWindow
{
public:

    AdvisorWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice,
        std::string _text,std::string _filepath,Vector2 _setScale);
    
    ~AdvisorWindow() override;
    
    void update(GameData* _gameData, Vector2& _mousePosition) override;
    void render(DrawData2D* _drawData) override;
    void reSize(Vector2 game_res) override;

    void setText(string new_string);
    void setAriaImage(string filename);
    void setJasperImage(string filename);
    void setOptionBox(int box_num, int indicator, string title);
    
private:
    std::vector<ImageGO2D*> image_vec_ar;
    std::vector<ImageGO2D*> image_vec_ja;
    std::vector<ImageGO2D*> indicators_ar;
    std::vector<ImageGO2D*> indicators_ja;
    bool show_ind_ar[5];
    bool show_ind_ja[5];

    //filenames for adivsor images
    string advisor_filenames_ar[13] = { "ArA", "ArE", "ArL", "ArM", "ArO", "ArS", "ArBlink1", "ArBlink2", "ArBlink3", "ArTongue1", "ArTongue2", "ArTongue3", "ArIdle1"};
    string advisor_filenames_ja[12] = { "JaA", "JaC", "JaE", "JaH", "JaM", "JaY", "JaBlink1", "JaBlink2", "JaBlink3", "JaWings1", "JaWings2", "JaIdle1"};
    int pointed_image_ar = 12;
    int pointed_image_ja = 11;
};

