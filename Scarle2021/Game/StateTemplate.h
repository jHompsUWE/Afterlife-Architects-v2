#pragma once
#include <iostream>

#include "DataManager.h"
#include "NewEventManager.h"
#include "AudioManager.h"

/**
 * \brief This is an interface where all the
 * Game States inherit from
 */
class StateTemplate
{
public:
    virtual ~StateTemplate() = default;

    // Init
    virtual bool init() = 0;
    
    // Updates
    virtual void Update(GameData* game_data) = 0;
    virtual void ScaledUpdate(GameData* game_data, float& scaled_dt) = 0;
    virtual void LateUpdate(GameData* game_data) = 0;

    // Event List
    virtual void GetEvents(const AL::Event& al_event) = 0;
    
    // Render
    virtual void Render3D(DrawData* draw_data) = 0;
    virtual void Render2D(DrawData2D* draw_data2D) = 0;

    //Reset
    virtual void Resize() = 0;
    virtual void Reset() = 0;
};
