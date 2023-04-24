#include "pch.h"
#include "AdvisorManager.h"
#include <iostream>

AdvisorManager::AdvisorManager()
{
    std::ifstream file("../Game/JSON_Files/AdvisorDialogues.json");
    json_values = nlohmann::json::parse(file);
    AL::NewEventManager::AddEventReceiver(false, this, AL::EventType::event_ui, AL::EventType::event_adv_fault);
}

AdvisorManager::~AdvisorManager()
{
    AL::NewEventManager::RemoveEventReceiver(this);
}

/// <summary>
/// Initialise advisors with UI ref
/// </summary>
/// <returns></returns>
bool AdvisorManager::init(AdvisorWindow* adv_wind)
{
    LoadFromJson();
    srand(time(0));
    advisor_window = adv_wind;
    
    UpdateButtons();
    return 0;
}

/// <summary>
/// Update the manager each frame to check if advise needs to be displayed or updated
/// </summary>
/// <param name="game_data"></param>
void AdvisorManager::Update(GameData* game_data)
{
    if (dia_state == Talking)
    {
        // Code for update whilst talking
        if (dia_time_tracker >= dia_time_scale * dia_string_length)
        {
            // If dialogue should end after timer
            if (dia_array_pointers[dia_current_index] != -1)
            {
                // If there is more dialogue to be displayed
                GenerateAdvise(dia_array_pointers[dia_current_index]);
            }
            else
            {
                // If not more dialogue is after
                dia_state = Finished;
                // Clear textbox
                advisor_window->setText("");
            }
        }
        else
        {
            // If dialogue is still going
            dia_time_tracker += game_data->delta_time;
        }
    }

    AnimationUpdate();
}

/// <summary>
/// Get advise event from Event Manager
/// </summary>
/// <param name="al_event"></param>
const bool& AdvisorManager::ReceiveEvents(const AL::Event& al_event)
{
    int index = -1;
    switch (al_event.type)
    {
    case AL::event_ui:
        switch (al_event.ui.action)
        {
        case AL::UI::adv_option1:
            index = 0;
            break;

        case AL::UI::adv_option2:
            index = 1;
            break;

        case AL::UI::adv_option3:
            index = 2;
            break;

        case AL::UI::adv_option4:
            index = 3;
            break;

        case AL::UI::adv_option5:
            index = 4;
            break;

        default:
            break;;
        }
        if (index != -1)
        {
            if (current_faults[index] != -1)
            {
                GenerateAdvise(dialogue_starts[current_faults[index]]);
            }
        }
        break;
    case AL::event_adv_fault:
        // Add or remove fault based off of event bool
        if (al_event.advisor.add_fault)
        {
            AddFault(al_event.advisor.fault_codename);
        }
        else
        {
            RemoveFault(al_event.advisor.fault_codename);
        }
        break;
    }
    return false;
}

/// <summary>
/// Update both Aria and Jasper images to animate them
/// </summary>
void AdvisorManager::AnimationUpdate()
{
    if (dia_state == Talking)
    {
        // If Jasper is talking
        if (dia_array_advisor[dia_current_index] == Jasper)
        {
            // Update img based on char for Jasper
            TalkingJasper();
            // Update img idle for Aria
            IdleAria();
        }
        // If Aria is talking
        else
        {
            // Update img based on char for Aria
            TalkingAria();
            // Update img idle for Jasper
            IdleJasper();
        }
    }
    else
    {
        // If neither are talking

        // Update img idle for Aria
        IdleAria();
        // Update img idle for Jasper
        IdleJasper();
    }
}

/// <summary>
/// Start the advise of a certain index
/// </summary>
/// <param name="index"></param>
void AdvisorManager::GenerateAdvise(int index)
{
    dia_time_tracker = 0;
    dia_current_index = index;
    advisor_window->setText(dia_array_string[index]);
    // CHANGE TEXT COLOUR BASED ON ARIA OR JASPER
    dia_string_length = dia_array_string[index].length();
    dia_state = Talking;
}

/// <summary>
/// Play animation for Aria based on idle sprites
/// </summary>
void AdvisorManager::IdleAria()
{
    aria_blink_timer-=1;
    aria_tongue_timer-=1;

    if (aria_blink_timer <= 0)
    {
        aria_blink_timer = 600 + (rand() % 600);
    }
    else if (aria_blink_timer <= 5)
    {
        advisor_window->setAriaImage("ArBlink3");
    }
    else if (aria_blink_timer <= 10)
    {
        advisor_window->setAriaImage("ArBlink2");
    }
    else if (aria_blink_timer <= 15)
    {
        advisor_window->setAriaImage("ArBlink1");
    }
    else if (aria_tongue_timer <= 0)
    {
        aria_tongue_timer = 1200 + (rand() % 600);
    }
    else if (aria_tongue_timer <= 10)
    {
        advisor_window->setAriaImage("ArTongue2");
    }
    else if (aria_tongue_timer <= 20)
    {
        advisor_window->setAriaImage("ArTongue3");
    }
    else if (aria_tongue_timer <= 30)
    {
        advisor_window->setAriaImage("ArTongue2");
    }
    else if (aria_tongue_timer <= 40)
    {
        advisor_window->setAriaImage("ArTongue1");
    }
    else
    {
        advisor_window->setAriaImage("ArIdle1");
    }
}

/// <summary>
/// Play animation for Aria based on talking sprites
/// </summary>
void AdvisorManager::TalkingAria()
{
    // Get position of character (at current time in dialogue) in the alphabet
    int index = GetCharIndex();
    if (index < 0)
    {
        advisor_window->setAriaImage("ArIdle1");
    }
    else
    {
        advisor_window->setAriaImage(aria_images[aria_img_pointers[index]]);
    }
}

/// <summary>
/// Play animation for Jasper based on idle sprites
/// </summary>
void AdvisorManager::IdleJasper()
{
    jasper_blink_timer--;
    jasper_wing_timer--;
    if (jasper_blink_timer <= 0)
    {
        jasper_blink_timer = 600 + (rand() % 600);
    }
    else if (jasper_blink_timer <= 5)
    {
        advisor_window->setJasperImage("JaBlink3");
    }
    else if (jasper_blink_timer <= 10)
    {
        advisor_window->setJasperImage("JaBlink2");
    }
    else if (jasper_blink_timer <= 15)
    {
        advisor_window->setJasperImage("JaBlink1");
    }
    else if (jasper_wing_timer <= 0)
    {
        jasper_wing_timer = 1200 + (rand() % 600);
    }
    else if (jasper_wing_timer <= 10)
    {
        advisor_window->setJasperImage("JaWings1");
    }
    else if (jasper_wing_timer <= 20)
    {
        advisor_window->setJasperImage("JaWings2");
    }
    else if (jasper_wing_timer <= 30)
    {
        advisor_window->setJasperImage("JaWings1");
    }
    else
    {
        advisor_window->setJasperImage("JaIdle1");
    }
}

/// <summary>
/// Play animation for Jasper based on speaking sprites
/// </summary>
void AdvisorManager::TalkingJasper()
{
    // Get position of character (at current time in dialogue) in the alphabet
    int index = GetCharIndex();
    if (index < 0)
    {
        advisor_window->setJasperImage("JaIdle1");
    }
    else
    {
        advisor_window->setJasperImage(jasper_images[jasper_img_pointers[index]]);
    }
}

/// <summary>
/// Stop and remove any current advise
/// </summary>
void AdvisorManager::StopAdvise()
{
    dia_time_tracker = 0;
    advisor_window->setText("");
    dia_state = Finished;
}

/// <summary>
/// Get position of character in the alphabet
/// </summary>
/// <returns></returns>
int AdvisorManager::GetCharIndex()
{
    int int_temp = (int)(dia_time_tracker / dia_time_scale);
    char char_temp = dia_array_string[dia_current_index][int_temp];
    // If space return -1 for idle sprite
    if (char_temp == ' ')
    {
        return -1;
    }
    char_temp = toupper(char_temp);
    int_temp = (int)char_temp - 65;
    return int_temp;
}

/// <summary>
/// Add new fault to fault list dependant on an event
/// </summary>
/// <param name="index"></param>
void AdvisorManager::AddFault(string codename)
{
    // Convert codename into int
    int index = -1;
    for (int i = 0; i < json_values["advisorDialogues"].size(); i++)
    {
        if (json_values["advisorDialogues"][i]["codeName"].get<string>() == codename)
        {
            index = i;
        }
    }
    if (index == -1)
    {
        return;
    }
    // Checks if fault is already shown
    bool contains_fault = false;
    int last_point = -1;
    for (int i = 0; i < 5; i++)
    {
        if (current_faults[i] == index)
        {
            contains_fault = true;
        }
        if (current_faults[i] != -1)
        {
            last_point = i;
        }
    }

    // If fault is not already shown and there is space to show this fault
    if (!contains_fault && current_faults[4] == -1)
    {
        current_faults[last_point + 1] = index;
        UpdateButtons();
    }
}

/// <summary>
/// Remove an existing fault of the list dependant on an event
/// </summary>
/// <param name="index"></param>
void AdvisorManager::RemoveFault(string codename)
{
    // Convert codename into int
    int index = -1;
    for (int i = 0; i < json_values["advisorDialogues"].size(); i++)
    {
        if (json_values["advisorDialogues"][i]["codeName"].get<string>() == codename)
        {
            index = i;
        }
    }
    if (index == -1)
    {
        return;
    }
    // Checks if fault is already shown
    bool contains_fault = false;
    int fault_point = 0;
    for (int i = 0; i < 5; i++)
    {
        if (current_faults[i] == index)
        {
            contains_fault = true;
            fault_point = i;
        }
    }

    // If fault is not already shown and there is space to show this fault
    if (contains_fault)
    {
        for (int j = fault_point; j < 4; j++)
        {
            current_faults[j] = current_faults[j + 1];
        }
        current_faults[4] = -1;
        UpdateButtons();
    }
}

/// <summary>
/// Returns true or false based on if it contains the fault specified
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
bool AdvisorManager::ContainsFault(int index)
{
    for (int i = 0; i < 5; i++)
    {
        if (current_faults[i] == index)
        {
            return true;
        }
    }
    return false;
}

/// <summary>
/// Update all option buttons after fault addition or removal
/// </summary>
void AdvisorManager::UpdateButtons()
{
    for (int i = 0; i < 5; i++)
    {
        if (current_faults[i] != -1)
        {
            advisor_window->setOptionBox(i, dialogue_standpoints[current_faults[i]], dialogue_titles[current_faults[i]]);
        }
        else
        {
            advisor_window->setOptionBox(i, Neither, "");
        }
    }
}

/// <summary>
/// Load all dialogue oriented variables from JSON
/// </summary>
void AdvisorManager::LoadFromJson()
{
    int index_tracker = 0;
    for (int d = 0; d < json_values["advisorDialogues"].size(); d++)
    {
        dialogue_starts.push_back(index_tracker);
        dialogue_titles.push_back(json_values["advisorDialogues"][d]["dialogueTitle"].get<string>());
        dialogue_codenames.push_back(json_values["advisorDialogues"][d]["codeName"].get<string>());
        switch (json_values["advisorDialogues"][d]["standpoint"].get<int>())
        {
        case 0:
            // Heaven
            dialogue_standpoints.push_back(Adv_Heaven);
            break;

        case 1:
            // Hell
            dialogue_standpoints.push_back(Adv_Hell);
            break;

        case 2:
            // Both
            dialogue_standpoints.push_back(Both);
            break;

        case 3:
            // Neither
            dialogue_standpoints.push_back(Neither);
            break;
        }
        int total_dias = json_values["advisorDialogues"][d]["texts"].size();
        for (int i = 0; i < total_dias; i++)
        {
            string temp_string = json_values["advisorDialogues"][d]["texts"][i].get<string>();
            dia_array_string.push_back(AddSpaces(temp_string));
            if (json_values["advisorDialogues"][d]["whosTalking"][i].get<int>() == 0)
            {
                // Jasper
                dia_array_advisor.push_back(Jasper);
            }
            else
            {
                // Aria
                dia_array_advisor.push_back(Aria);
            }
            if (i == total_dias - 1)
            {
                dia_array_pointers.push_back(-1);
            }
            else
            {
                dia_array_pointers.push_back(index_tracker + i + 1);
            }
        }
        index_tracker+= total_dias;
    }
}

std::string AdvisorManager::AddSpaces(std::string inp_string)
{
    std::string new_string;
    int letter_tracker = 0;
    for (auto& letter : inp_string)
    {
        new_string+=letter;
        letter_tracker++;
        if (letter_tracker > 30)
        {
            if (letter == ' ')
            {
                new_string += "\n";
                letter_tracker = 0;
            }
        }
    }
    return new_string;
}