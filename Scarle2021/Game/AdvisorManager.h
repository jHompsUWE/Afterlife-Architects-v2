#pragma once
#include "DataManager.h" 
#include "NewEventManager.h"
#include "AdvisorWindow.h"
#include <unordered_map>
#include <fstream>
#include <memory>
#include "../json/single_include/nlohmann/json.hpp"

enum DialogueState
{
    Talking, Finished
};

enum Advisor
{
    Jasper, Aria
};

enum HeavenOrHell
{
    Adv_Heaven, Adv_Hell, Both, Neither
};

class AdvisorManager : public AL::EventReceiver
{
public:
    AdvisorManager();
    ~AdvisorManager() override;

    bool init(AdvisorWindow* adv_wind);

    // Update 
    void Update(GameData* game_data);
    
    //Events
    const bool& ReceiveEvents(const AL::Event& al_event) override;

private:
    void LoadFromJson();

    // Starting, updating and stopping advise
    void StopAdvise();
    void AnimationUpdate();
    void GenerateAdvise(int index);

    // Animation handling
    void IdleJasper();
    void TalkingJasper();
    void IdleAria();
    void TalkingAria();
    
    // Fault handling
    void AddFault(string codename);
    void RemoveFault(string codename);
    void UpdateButtons();
    bool ContainsFault(int index);

    int GetCharIndex();

    AdvisorWindow* advisor_window = nullptr;

    DialogueState dia_state = Finished;
    float dia_time_tracker = 0;
    int dia_string_length = 0;
    int dia_current_index = -1;
    float dia_time_scale = 0.15; // Speed of dialogue

    // ----------------ANIMATION-----------------
    // All images for Jasper dialogue (CHARACTERS)
    string jasper_images[20] = { "JaA", "JaC", "JaE", "JaH", "JaM", "JaY" };
    // Index for what character is what image
    int jasper_img_pointers[26] = { 0 , 4 , 1 , 1 , 2 , 1 , 5 , 3 , 0 , 5 , 1 , 5 , 4 , 4 , 5 , 2 , 5 , 0 , 1 , 2 , 5 , 2 , 5 , 1 , 5 , 1 };

    // All images for Aria dialogue (CHARACTERS)
    string aria_images[20] = { "ArA", "ArE", "ArL", "ArM", "ArO", "ArS"};
    // Index for what character is what image
    int aria_img_pointers[26] = { 0 , 3 , 5 , 1 , 1 , 5 , 4 , 0 , 1 , 1 , 1 , 2 , 3 , 3 , 4 , 1 , 4 , 1 , 5 , 1 , 4 , 1 , 3 , 1 , 4 , 1 };
    // ------------------------------------------

    // Measured in in-game ticks
    int jasper_wing_timer = 1200; // Initial time between jasper wings animation
    int jasper_blink_timer = 600; // Initial time between jasper blink animatio
    int aria_tongue_timer = 1800; // Initial time between aria tongue animation
    int aria_blink_timer = 300; // Initial time between areai blink animation

    // Faults Titles, Index Starts, Planes
    vector<int> current_faults = { -1,-1,-1,-1,-1 };
    vector<int> dialogue_starts = {};
    vector<string> dialogue_titles = {};
    vector<HeavenOrHell> dialogue_standpoints = {};
    vector<string> dialogue_codenames = {};
    // String for every dialogue
    vector<string> dia_array_string = {};
    // NOT POINTERS but instead directs the next dialogue index
    vector<int> dia_array_pointers = {};
    // Dictates who is saying what
    vector<Advisor> dia_array_advisor = {};

    nlohmann::json json_values;
};

