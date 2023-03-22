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
	
	/**
	 * \return Instance of EventManager
	 */
	NewEventManager& NewEventManager::Get()
	{
		static NewEventManager instance;
		return instance;
	}
	
	// Static ----------------------------------------------------------------------------------------------------------

	/**
	 * \brief Static function to generate an event
	 * \tparam Payload Data to be inserted in the event
	 * \param type which type of even this is gonna be
	 */
	template <typename... Payload>
	void NewEventManager::GenerateEventSt(EventType type, const Payload&... args)
	{
		Get().GenerateEvent(type, args...);
	}
	
	/**
	 * \return static func to return the event list
	 */
	std::vector<Event>& NewEventManager::GetEventListSt()
	{
		return Get().GetEventList();
	}

	/**
	 * \brief Flushes the event list
	 */
	void NewEventManager::FlushEventListSt()
	{
		Get().FlushEventList();
	}

	/**
	 * \param observer to be subscribed to receive events
	 */
	void NewEventManager::AddEventReceiver(IEventReceiver* observer)
	{
		Get().AddObserver(observer);
	}

	/**
	 * \param observer to be unsubscribed from receiving events
	 */
	void NewEventManager::RemoveEventReceiver(IEventReceiver* observer)
	{
		Get().RemoveObserver(observer);
	}

	// Event List ------------------------------------------------------------------------------------------------------
	
	/**
	 * \return returns the event list.
	 */
	std::vector<Event>& NewEventManager::GetEventList()
	{
		return event_list;
	}

	/**
	 * \brief Clear all data from the event list
	 */
	void NewEventManager::FlushEventList()
	{
		event_list.clear();
	}

	// Data Sharing ----------------------------------------------------------------------------------------------------

	/**
	 * \brief Routes the events to each of the observers
	 */
	void NewEventManager::BroadcastData()
	{
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

	// Input Mapping ---------------------------------------------------------------------------------------------------

	/**
	 * \brief To map keyboard entries to events
	 * \param keyboard state in this current frame
	 */
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
	
	/**
	 * \brief to map mouse entries to events
	 * \param mouse state in this current frame
	 */
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

	/**
	 * \brief to map gamepad entries to events
	 * \param gamepad state in this current frame
	 */
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

	/**
	 * @brief Checks the position of the various pointing devices and generates an event accordingly
	 * Based on the last input registered, the input will be automatically set to controller or m&k
	 */
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

	// TODO::IMPLEMENT THIS PROPERLY
	// BUTTON WORK AROUND ----------------------------------------------------------------------------------------------
	
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

	// Event Generation ------------------------------------------------------------------------------------------------

	/**
	 * \brief Generates an event form the data given, easy as that
	 * \tparam Payload Data to be inserted in the event
	 * \param type which type of even this is gonna be
	 */
	template <typename ... Payload>
	void NewEventManager::GenerateEvent(EventType type, const Payload&... args)
	{
		//ATM are not in place any sort of memory checks on the size of data being moved inside an event
		//If the size of data surpasses 40 bytes a memory override will happen. and we *do not* want that
		event_list.emplace_back(type);
		SetEventData(event_list.back(), args...);
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

		//Maps the current states of the outputs to keep track of pressed and relesed actions
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

	// Event Data Insertion --------------------------------------------------------------------------------------------

	/**
	 * \brief This function takes and data, will the store all the data provided inside the event.
	 * \tparam Payload variadic template, this is the packet of values and types that need to be inserted
	 * \param event reference to the event
	 */
	template <typename... Payload>
	void NewEventManager::SetEventData(Event& event, const Payload&... args)
	{
		//Gets the inital data offset of the event and starts the population process
		int byte_offset = sizeof(EventType);
		SetEventData(event, byte_offset, args...);
	}
	
	/**
	 * \brief This is closely linked to the previous function and will keep recurring until all the data has been moved
	 * \tparam T type of the current type that is being inserted
	 * \tparam Payload packet or remaining data 
	 * \param event reference to the event
	 * \param byte_offset offset of bytes from the initial memory address 
	 */
	template <typename T, typename ... Payload>
	void NewEventManager::SetEventData(Event& event, int& byte_offset, const T& arg, const Payload&... args)
	{
		//Check the current allocation of memory to not overshoot
		if(byte_offset + sizeof(T) > sizeof(Event))
		{
			std::cout << "Data inserted overshoots memory limit for Event, discarding data" << std::endl;
			return;
		}
		
		//Checks if the value is char
		if constexpr (std::is_same<typename std::remove_cv<typename std::remove_extent<T>::type>::type, char>::value)
		{
			//If it is, save its size and save it as a char*
			const int size = sizeof(event.sound_start.filename);
			const char* string = arg;
			size_t len = strnlen_s(string, size); //gets len of the string

			//Checks if the string is small enough to fit into a char[32]
			if (len < size)
			{

				strcpy_s(*(char(*)[sizeof(T)])((char*)&event + byte_offset), arg);
			}
			else
			{
				//string too big! not good!
				std::cout << "This event value has not been saved, value exceeds buffer." << std::endl;
			}

			//Adds the size of the string to the buffer for the next values
			byte_offset += size;
		}
		else
		{
			//If not a string cast value as T* and takes the correct place in memory
			//To move the pointer of memory a char* is used because it is a single byte
			//More appropriate would be using std::byte, but that is not introduced yet in c++ 14
			*(T*)((char*)&event + byte_offset) = arg;
			byte_offset += sizeof(T);
		}

		//Keep recurring until all the value in the data package are used
		if constexpr (sizeof...(args) > 0)
		{
			SetEventData(event, byte_offset, args...);
		}
	}

	//Cursor Getters ---------------------------------------------------------------------------------------------------

	Vector2 NewEventManager::GetCursorPos() const
	{
		return Vector2{(float)cursor_pos.x, (float)cursor_pos.y};
	}

	void NewEventManager::SetSpriteSpeed(const Vector2& new_speed)
	{
		cursor_speed *= new_speed.x;
	}

	//Template function specialization ---------------------------------------------------------------------------------
	
	//Input Event
	template void NewEventManager::GenerateEventSt<Input::Action, bool>(EventType, const Input::Action&, const bool&);
	//Cursor Event Move
	template void NewEventManager::GenerateEventSt<int, int>(EventType, const int&, const int&);
	//Cursor Event Interact
	template void NewEventManager::GenerateEventSt<Cursor::Action, bool>(EventType, const Cursor::Action&, const bool&);
	//Sound Event Start
	template void NewEventManager::GenerateEventSt<char[32], float, bool>(EventType, const char(&)[32], const float&, const bool&);
	//Sound Event Stop
	template void NewEventManager::GenerateEventSt<char[32]>(EventType, const char(&)[32]);
	//Interface Event
	template void NewEventManager::GenerateEventSt<UI::Action>(EventType, const UI::Action&);
	//Building System Event
	template void NewEventManager::GenerateEventSt<BuildSys::Section, StructureType, ZoneType>(
		EventType type, const BuildSys::Section&, const StructureType&, const ZoneType&);
	//Game Event
	template void NewEventManager::GenerateEventSt<Game::Action>(EventType, const Game::Action&);
	//Advisor Fault Event
	template void NewEventManager::GenerateEventSt<int>(EventType, const int&);
	//Camera Event
	template void NewEventManager::GenerateEventSt<int>(EventType, const int&, const int&);
}
