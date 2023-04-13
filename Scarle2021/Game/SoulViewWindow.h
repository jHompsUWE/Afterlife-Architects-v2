#pragma once
#include "UIWindow.h"
#include "PopulationManager.h"

class SoulViewWindow : public IEventReceiver
{
public:

    SoulViewWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice,
        std::string _text, std::string _filepath, Vector2 _setScale, std::shared_ptr<PopulationManager> pop_manager);

    ~SoulViewWindow() override;

    void update(GameData* _gameData, Vector2& _mousePosition);
    void render(DrawData2D* _drawData);
    void ReceiveEvents(const AL::Event& al_event) override;

    void set_postion(Vector2& _new_pos);
    void set_scale(Vector2& _newScale);

    void generateRandSoul();

    Vector2& getPosition();
    Vector2& getButtonRes();

    void reSize(Vector2 game_res);
    bool is_visible = false;

private:

    string generateZone(PlaneType plane);
    string generateBelief(PlaneType plane);

    //mouse pointer inside window
    bool isInside(Vector2& point) const;
    bool toggle_click = false;
    Vector2 mouse_pos = { 0,0 };
    Vector2 old_mouse_pos{ 0,0 };

    ImageGO2D* windowBackGround = nullptr;
    std::shared_ptr<PopulationManager> population_manager;

    //vector of buttons 
    std::vector<UIButtonInterFace*> buttons;
    std::vector<TextGO2D*> text_vec;
    std::vector<ImageGO2D*> image_vec;

    string random_names[6] = { "Oscar Wilkinson","Scott Knapp","Elvin Nicholson","Simone Farinelli","Awse Noori", "Seb LLewellyn" };
    string available_punishments[14] = { "Envy","Avarice","Gluttony","Sloth","Pride","Wrath","Lust","Contentment","Charity","Temperance","Diligence","Humility","Peacefulness","Chastity" };

    // NAAA does not show in afterlife
    // AAAA does believe in afterlife
    // NOT USED
    string beliefOne[4] = { "HAHA", "HOHO", "OCRA", "OPRA" };
    // HAHA serve in hell, then heaven
    // HOHO Only serve in one realm depending on balance
    // OCRA only in heaven
    // OPRA only in hell
    string beliefTwo[2] = { "ALF" , "RALF" };
    // ALF do not reincarnate
    // RALF do reincarnate
    string beliefThree[2] = { "SUMA", "SUSA" };
    // SUMA serve all sins and virtues (multiple)
    // SUSA serve one sin or virtue (single)

    //vectors
    Vector2 window_res{ 0,0 };
    Vector2 window_pos{ 0,0 };
};


