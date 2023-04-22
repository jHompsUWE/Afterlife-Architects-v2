#pragma once
#include "UIWindow.h"
#include "PopulationManager.h"

class SoulViewWindow : public UIWindow
{
public:

    SoulViewWindow(Vector2 _windowPosition, ID3D11Device* _d3dDevice,
        std::string _text, std::string _filepath, Vector2 _setScale, std::shared_ptr<PopulationManager> pop_manager);

    ~SoulViewWindow() override;

    void update(GameData* _gameData, Vector2& _mousePosition) override;
    void render(DrawData2D* _drawData) override;
    void reSize(Vector2 game_res) override;
    
    void generateRandSoul();
    
private:
    string generateZone(PlaneType plane);
    string generateBelief(PlaneType plane);
    std::shared_ptr<PopulationManager> population_manager;

    //Images
    std::vector<ImageGO2D*> image_vec;

    // Strings for all names, punsihment and religions
    const std::vector<string> random_names = { "Oscar Wilkinson","Scott Knapp","Elvin Nicholson","Simone Farinelli", "Seb LLewellyn", "Klan'rkkliaktu", "Trak'takli", "Klak'takluk'ta", "T'klaaktrkla", "Klun'rkkluaktu"};
    const std::vector<string> available_punishments = { "Envy","Avarice","Gluttony","Sloth","Pride","Wrath","Lust","Contentment","Charity","Temperance","Diligence","Humility","Peacefulness","Chastity" };
    const std::vector<string> beliefOne = { "HAHA", "HOHO", "OCRA", "OPRA" };
    const std::vector<string> beliefTwo = { "ALF" , "RALF" };
    const std::vector<string> beliefThree = { "SUMA", "SUSA" };
};


