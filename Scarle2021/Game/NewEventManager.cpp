#include "pch.h"
#include "NewEventManager.h"

#include <iostream>

#include "DataManager.h"

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
	
	void NewEventManager::AddEventReceiver(EventReceiver* receiver)
	{
		Get().AddReceiver(receiver);
	}
	
	void NewEventManager::RemoveEventReceiver(EventReceiver* receiver)
	{
		Get().RemoveReceiver(receiver);
	}

	std::vector<Event>& NewEventManager::GetEventListSt()
	{
		return Get().GetEventList();
	}
	
	void NewEventManager::FlushEventListSt()
	{
		Get().FlushEventList();
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
	
	void NewEventManager::DispatchEvents()
	{
		//Empties the event list inside the dispatch list
		std::vector<Event> dispatch_list = std::move(event_list);

		//Iterates through the events but backwards
		for (int i = dispatch_list.size() - 1; i >= 0; --i)
		{
			auto& current_ev = dispatch_list[i];

			//Fires the event if the delay is minus equals 0
			if (current_ev.delay <= 0)
			{
				//Event is dispatched to each observer
				for (const auto& receiver : receiver_list)
				{
					//Fires the event only if the receiver matches the type it is listening for 
					if(receiver.first == current_ev.type)
					{
						receiver.second->ReceiveEvents(current_ev);
					}
				}

				//After dispatch the event is erased
				dispatch_list.erase(dispatch_list.begin() + i);
			}
			else
			{
				//Decreases delay
				current_ev.delay -= DataManager::GetGD()->delta_time;
			}
		}

		//Merges the unfired events in the dispatch list with the newly generated and going to be generated events
		event_list.insert(event_list.end(), dispatch_list.begin(), dispatch_list.end());
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
