#include "pch.h"
#include "NewEventManager.h"

#include <iostream>

namespace AL
{
	//Default private constructor, no need to initialize inherited class
	NewEventManager::NewEventManager() = default;
	
	//Default private de-constructor
	NewEventManager::~NewEventManager() = default;

	// Instance --------------------------------------------------------------------------------------------------------
	
	//Gets singleton instance
	NewEventManager& NewEventManager::Get()
	{
		static NewEventManager instance;
		return instance;
	}
	
	// Static ----------------------------------------------------------------------------------------------------------
	
	std::vector<Event>& NewEventManager::GetEventListSt()
	{
		return Get().GetEventList();
	}
	
	void NewEventManager::FlushEventListSt()
	{
		Get().FlushEventList();
	}
	
	void NewEventManager::AddEventReceiver(IEventReceiver* observer)
	{
		Get().AddObserver(observer);
	}
	
	void NewEventManager::RemoveEventReceiver(IEventReceiver* observer)
	{
		Get().RemoveObserver(observer);
	}

	// Event List ------------------------------------------------------------------------------------------------------
	
	std::vector<Event>& NewEventManager::GetEventList()
	{
		return event_list;
	}
	
	void NewEventManager::FlushEventList()
	{
		event_list.clear();
	}

	// Data Sharing ----------------------------------------------------------------------------------------------------
	
	void NewEventManager::BroadcastData()
	{
		//Gets a copy to prevent messing with the loop
		const auto event_list_copy = event_list;
		
		FlushEventList();

		//Batch of events N1
		for (const auto& al_event : event_list_copy)
		{
			for (const auto& observer : observers)
			{
				observer->ReceiveEvents(al_event);
			}
		}

		//Batch of events n2
		for (const auto& al_event : event_list)
		{
			for (const auto& observer : observers)
			{
				observer->ReceiveEvents(al_event);
			}
		}
		
		FlushEventList();
	}

	// Input Polling ---------------------------------------------------------------------------------------------------
	
	void NewEventManager::PollKeyboard(Keyboard::State keyboard)
	{
		//Do not map keybindings if controller is being used 
		if(controller_active) return;
		
		//Map keyboard keys here
		///TEMPORARY
		MapEntryToEvent(keyboard.E, Input::build_houses);
		MapEntryToEvent(keyboard.P, Input::show_vibes);
		MapEntryToEvent(keyboard.D1, Input::place_zone_green);

		//Camera movement linked to keyboard
		MapEntryToEvent(keyboard.W,Input::camera_up, true);
		MapEntryToEvent(keyboard.S,Input::camera_down, true);
		MapEntryToEvent(keyboard.A,Input::camera_left, true);
		MapEntryToEvent(keyboard.D,Input::camera_right, true);
	}
	
	void NewEventManager::PollMouse(Mouse::State mouse)
	{
		//Creates event for mouse scroll if necessary
		MouseScrollToEvent(mouse);

		//Maps mouse_pos to the mouse state pos
		mouse_pos = {mouse.x, mouse.y};

		//Do not map keybindings if controller is being used 
		if(controller_active) return;
		
		//Map mouse keys here
		MapEntryToEvent(mouse.leftButton, Cursor::button_input1);
		MapEntryToEvent(mouse.rightButton, Cursor::button_input2);
		MapEntryToEvent(mouse.middleButton, Cursor::button_input3);
	}
	
	void NewEventManager::PollGamepad(GamePad::State gamepad, const float& dt)
	{
		if(gamepad.IsConnected())
		{
			//Gathers all the input axis from the first controller connected
			
			//Comtroller Thumbs-ticks int value 0 - 100
			Vector2Int thumb_l = {static_cast<int>(gamepad.thumbSticks.leftX * 100),
								  static_cast<int>(gamepad.thumbSticks.leftY * 100)};
			//Applies a dead-zone
			thumb_l.x = thumb_l.x > dead_zone || thumb_l.x < -dead_zone ? thumb_l.x : 0;
			thumb_l.y = thumb_l.y > dead_zone || thumb_l.y < -dead_zone ? thumb_l.y : 0;
			
			Vector2Int thumb_r = {static_cast<int>(gamepad.thumbSticks.rightX * 100),
								  static_cast<int>(gamepad.thumbSticks.rightY * 100)};
			//Applies a dead-zone
			thumb_r.x = thumb_r.x > dead_zone || thumb_r.x < -dead_zone ? thumb_r.x : 0;
			thumb_r.y = thumb_r.y > dead_zone || thumb_r.y < -dead_zone ? thumb_r.y : 0;

			//Controller triggers int value 0 - 100
			Vector2Int triggers = {static_cast<int>(gamepad.triggers.left * 100),
								   static_cast<int>(gamepad.triggers.right * 100)};
			//Applies a dead-zone
			triggers.x = triggers.x > dead_zone || triggers.x < -dead_zone ? triggers.x : 0;
			triggers.y = triggers.y > dead_zone || triggers.y < -dead_zone ? triggers.y : 0;

			//Left thumbstick axis are mapped to cursor movement
			controller_pos.x += thumb_l.x * cursor_speed;
			controller_pos.y -= thumb_l.y * cursor_speed;

			//Right thumbstick axis are mapped to camera movement
			if(thumb_r.x != 0 || thumb_r.y != 0)
			{
				//Sends the Y and X axis of thumbstick right to the camera when
				//they are not 0
				GenerateEvent(event_camera, thumb_r.x, thumb_r.y);
			}
			
			//Do not map specific controller keybindings if mouse is active
			if(!controller_active) return;
			
			//Cursor interaction 
			if(triggers.x > 0.8f || triggers.y > 0.8f)
			{
				MapEntryToEvent(true, Cursor::button_input1);
			}
			else
			{
				MapEntryToEvent(false, Cursor::button_input1);
			}
			
			//Camera zoom
			MapEntryToEvent(gamepad.IsLeftShoulderPressed(), Cursor::Action::scroll_down);
			MapEntryToEvent(gamepad.IsRightShoulderPressed(), Cursor::Action::scroll_up);
		}
	}
	
	void NewEventManager::UpdateCursorPos(const int& window_width, const int& window_height)
	{
		//Check if mouse position has changed
		if(prev_mouse_pos != mouse_pos)
		{
			prev_mouse_pos = mouse_pos;
			cursor_pos = mouse_pos;

			controller_active = false;
		}
		
		//Chech if the controller pointer has been moved
		if(controller_pos != prev_controller_pos)
		{
			prev_controller_pos = controller_pos;
			cursor_pos = controller_pos;

			controller_active = true;
		}

		//If the cursor pos has changed, send an event
		if(cursor_pos != prev_cursor_pos)
		{
			//Caps cursor inside the X axis of the screen
			if(cursor_pos.x > window_width) cursor_pos.x = window_width;
			else if(cursor_pos.x < 0) cursor_pos.x = 0;

			//Caps the cursor inside the Y axis of the screen 
			if(cursor_pos.y > window_height) cursor_pos.y = window_height;
			else if(cursor_pos.y < 0) cursor_pos.y = 0;
			
			prev_cursor_pos = cursor_pos;
			
			//Syncs the mouse input to 
			controller_pos = cursor_pos;
			prev_controller_pos = cursor_pos;

			//Generate Event accordingly
			GenerateEvent(event_cursor_move, cursor_pos.x, cursor_pos.y);
		}
	}
	
	// BUTTON WORK AROUND ----------------------------------------------------------------------------------------------
	
	// TODO::IMPLEMENT THIS PROPERLY
	void NewEventManager::GenerateEventSoundStart(const char filename[32], const float& volume, const bool& loop)
	{
		GenerateEvent(event_sound_start, filename, volume, loop);
	}

	void NewEventManager::GenerateEventSoundStop(const char filename[32])
	{
		GenerateEvent(event_sound_stop, filename);
	}

	void NewEventManager::GenerateInterfaceEvent(const UI::Action& action)
	{
		GenerateEvent(event_ui, action);
	}

	void NewEventManager::GenerateBuildSysEvent(const BuildSys::Section& section, const StructureType& structure,
		const ZoneType& zone)
	{
		GenerateEvent(event_build_sys, section, structure, zone);
	}

	void NewEventManager::GenerateGameEvent(const Game::Action& action)
	{
		GenerateEvent(event_game, action);
	}

	// Key Mapping Handlers --------------------------------------------------------------------------------------------

	void NewEventManager::MouseScrollToEvent(const Mouse::State& mouse)
	{
		//Gets current scroll value
		const int current_scroll = mouse.scrollWheelValue;

		//Has the scroll value changed?
		if(current_scroll != mouse_scroll)
		{
			if(current_scroll > mouse_scroll)
			{
				GenerateEvent(event_cursor_interact, Cursor::Action::scroll_up);
			}
			else if(current_scroll < mouse_scroll)
			{
				GenerateEvent(event_cursor_interact, Cursor::Action::scroll_down);
			}
			//Save the new scroll value 
			mouse_scroll = current_scroll;
		}
	}

	void NewEventManager::MapEntryToEvent(bool state, Input::Action action, bool repeat)
	{
		//If the input is set as repeat, generate an input event each poll action while
		//the button is being pressed
		if(repeat)
		{
			if(state)
			{
				//Repeat event
				GenerateEvent(event_input, action, state);
			}
			return;
		}

		//Maps the current states of the outputs to keep track of pressed and released actions
		if(state)
		{
			if(!input_to_action_map[action])
			{
				input_to_action_map[action] = true;
				//Button is pressed
				GenerateEvent(event_input, action, state);

			}
		}
		else
		{
			if(input_to_action_map[action])
			{
				input_to_action_map[action] = false;
				//Button is released 
				GenerateEvent(event_input, action, state);
			}
		}
	}

	void NewEventManager::MapEntryToEvent(bool state, Cursor::Action action, bool repeat)
	{
		//If the input is set as repeat, generate an input event each poll action while
		//the button is being pressed
		if(repeat)
		{
			if(state)
			{
				//Repeat Event
				GenerateEvent(event_cursor_interact, action, state);
			}
			return;
		}

		//Maps the current states of the outputs to keep track of pressed and relesed actions
		if(state)
		{
			if(!cursor_to_action_map[action])
			{
				cursor_to_action_map[action] = true;
				//Button is pressed 
				GenerateEvent(event_cursor_interact, action, state);
			}
		}
		else
		{
			if(cursor_to_action_map[action])
			{
				cursor_to_action_map[action] = false;
				//Button is released 
				GenerateEvent(event_cursor_interact, action, state);

			}
		}
	}
	
	//Cursor Getters ---------------------------------------------------------------------------------------------------

	SimpleMath::Vector2 NewEventManager::GetCursorPos() const
	{
		//Vector 2 for ease, casted before returning
		return SimpleMath::Vector2{static_cast<float>(cursor_pos.x), static_cast<float>(cursor_pos.y)};
	}

	void NewEventManager::SetSpriteSpeed(const SimpleMath::Vector2& new_speed)
	{
		cursor_speed *= new_speed.x;
	}
}
