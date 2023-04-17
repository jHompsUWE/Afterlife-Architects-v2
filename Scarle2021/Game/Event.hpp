﻿#pragma once

#include "ActionListCursor.h"
#include "ActionListInput.h"
#include "ActionListInterface.h"
#include "ActionListGame.h"
#include "ActionListBuildSys.h"
#include "ActionListBadThings.h"
#include "ZoneType.h"
#include "StructureType.h"

namespace AL
{
    //Type of events
    enum EventType
    {
        unknown = 0,
        
        event_input,
        event_cursor_move,
        event_cursor_interact,
        event_sound_start,
        event_sound_stop,
        event_ui,
        event_build_sys,
        event_game,
        event_adv_fault,
        event_camera,
        event_bad_things,

        last_entry
    };

    //Normal input event
    class Event
    {
    public:
        //Explicit constructor as event type is needed for sorting
        explicit Event(EventType _type, float _delay = 0) : type(_type), delay(_delay){}
        ~Event() = default;

        /**
         * \brief Event is fired for input entries that are mapped to this Event
         * \var action = action to process 
         * \var active = is button currently being pressed
         */
        struct InputEvent
        {
            Input::Action action = Input::Action::unknown;
            bool active = false;
        };

        /**
         * \brief Event is fired when the position of the cursor has changed
         */
        struct CursorEventMove
        {
            int pos_x = 0;
            int pos_y = 0;
        };
        
        /**
         * \brief Event is fired when a "interact" action is performed
         * \var action = action to process 
         * \var active = if the action should be handled as a start or finish of this event
         */
        struct CursorEventInteract
        {
            Cursor::Action action = Cursor::Action::unknown;
            bool active = 0;
        };
        
        /**
         * \brief Event to start to play a sound
        * \var filename = filename as seen in the asset pipeline, no file extension, max 31 chars.
         * \var volume
         * \var loop = will it loop?
         */
        struct SoundEventStart
        {
            char filename[32] = "unknown";
            float volume = 0;
            bool loop = false;
        };

        /**
         * \brief Event to a sound from playing
         * \var filename = filename as seen in the asset pipeline, no file extension, max 31 chars.
         */
        struct SoundEventStop
        {
            char filename[32] = "unknown";
        };

        /**
         * \brief Event to redirect an action to the UI
         */
        struct InterfaceEvent
        {
            UI::Action action = UI::Action::unknown;
        };

        /**
         * \brief Event to redirect an action to the UI
         * \var structure that is being instantiated
         * \var zone that is being instantiated
         */
        struct BuildSysEvent
        {
            BuildSys::Section section = BuildSys::Section::unknown;
        	StructureType structure = StructureType::unknown;
            ZoneType zone = null;
        };
        
        /**
         * \brief Event to be redirect an action to the game
         */
        struct GameEvent
        {
            Game::Action action = Game::Action::unknown;
        };

        /**
         * \brief Event to be redirect an action to the advisor
         */
        struct AdvisorEvent
        {
            int fault_index = 0;
            bool add_fault = false;
            // False is remove, True is Add
        };

        /**
         * \brief Event for camera movement
         */
        struct CameraEvent
        {
            int pos_x = 0;
            int pos_y = 0;
        };

        /**
         * \brief Event related to bad things
         * \var action = Bad thing in question
         * \var timer = Duration in seconds of the Event
         * \var start = If the event is starting or not
         */
        struct BadThingsEvent
        {
            BadThings::Action action = BadThings::unknown;
            float timer = 0.0f;
            bool start = false;
        };
        
        EventType type = unknown;
        float delay = 0;
        
        //Linking structs to keywords
        //A union will make it so that each element will share the same
        //memory, therefore making events lightweight
        union
        {
            InputEvent input;
            CursorEventMove cursor_moved;
            CursorEventInteract cursor_interact;
            SoundEventStart sound_start;
            SoundEventStop sound_stop;
            BuildSysEvent build_sys;
            InterfaceEvent ui;
            GameEvent game;
            AdvisorEvent advisor;
            CameraEvent camera;
        };
    };
}

