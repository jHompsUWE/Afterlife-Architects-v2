﻿#include "pch.h"
#include "NewEventManager.h"

#include <iostream>
#include <ostream>

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
	template <typename ... Payload>
	void NewEventManager::GenerateEventSt(EventType type, const Payload&... args)
	{
		Get().GenerateEvent(type, args);
	}

	/**
	 * \return static func to return the event list
	 */
	std::vector<Event>& NewEventManager::GetEventListSt()
	{
		return Get().GetEventList();
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

	// Input Mapping ---------------------------------------------------------------------------------------------------

	/**
	 * \brief To map keyboard entries to events
	 * \param keyboard state in this current frame
	 */
	void NewEventManager::PollKeyboard(Keyboard::State keyboard)
	{
		//Map keyboard keys here
		MapEntryToEvent(keyboard.W, Input::input_up);
		MapEntryToEvent(keyboard.S, Input::input_down);
		MapEntryToEvent(keyboard.A, Input::input_left);
		MapEntryToEvent(keyboard.D, Input::input_right);
	}
	
	/**
	 * \brief to map mouse entries to events
	 * \param mouse state in this current frame
	 */
	void NewEventManager::PollMouse(Mouse::State mouse)
	{
		//Creates event for mouse moved if necessary
		MouseMovToEvent(mouse);

		//Map mouse keys here
		MapEntryToEvent(mouse.leftButton, Cursor::button_input1);
		MapEntryToEvent(mouse.rightButton, Cursor::button_input2);
	}

	/**
	 * \brief to map gamepad entries to events
	 * \param gamepad state in this current frame
	 */
	void NewEventManager::PollGamepad(GamePad::State gamepad)
	{
		if(gamepad.IsConnected())
		{
			//Map controller behaviour here
			MapEntryToEvent(gamepad.IsAPressed(), Input::input_left);
		}
	}

	// Data Sharing ----------------------------------------------------------------------------------------------------

	/**
	 * \brief Routes the events to each of the observers
	 */
	void NewEventManager::BroadcastData()
	{
		for (const auto& entry : event_list)
		{
			//Event looping is already done here, no need for loop in observer
			for (const auto observer : observers)
			{
				observer->ReceiveEvents(entry);
			}
		}
	}

	// Key Mapping Handlers --------------------------------------------------------------------------------------------
	
	void NewEventManager::MouseMovToEvent(const Mouse::State& mouse)
	{
		//Check if mouse position has changed, and generate and event is so
		if(mouse.x != mouse_x || mouse.y != mouse_y)
		{
			mouse_x = mouse.x;
			mouse_y = mouse.y;

			GenerateEvent(event_cursor_move, mouse_x, mouse_y);
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

	// Event Data Insertion --------------------------------------------------------------------------------------------

	/**
	 * \brief This function takes and data, will the store all the data provided inside the event.
	 * \tparam Payload variadic template, this is the packet of values and types that need to be inserted
	 * \param event reference to the event
	 */
	template <typename ... Payload>
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
				//Copies the string, in this case, named arg, to the memory location defined
				//*(char(*)[sizeof(T)]) is used to cast that memory pointer to a char[] of
				//sizeof(T) value.
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
}
