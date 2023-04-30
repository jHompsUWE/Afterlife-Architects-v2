#pragma once

#include <iostream>

namespace AL
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// PUBLIC 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Static Event Generation -------------------------------------------------------------------------------------
	
    template <typename... Payload>
    void EventManager::GenerateEventSt(const EventType& type, const Payload&... args)
    {
        Get().GenerateEvent(type, args...);
    }

	template <typename... Payload>
	void EventManager::GenerateEventWithDelaySt(const EventType& type, const float& delay, const Payload&... args)
    {
    	Get().GenerateEventWithDelay(type, delay, args...);
    }

	// Static Filtered Subscriptions handlers ----------------------------------------------------------------------

	template <typename... Payload>
	void EventManager::AddEventReceiver(const bool& priority, EventReceiver* receiver, const Payload&... types)
    {
    	Get().AddReceiver(priority, receiver, types...);
    }

	template <typename... Payload>
	void EventManager::RemoveEventReceiver(EventReceiver* receiver, const Payload&... types)
    {
    	Get().RemoveReceiver(receiver, types...);
    }

	// Event Generation ------------------------------------------------------------------------------------------------
	
	template <typename ... Payload>
	void EventManager::GenerateEvent(EventType type, const Payload&... args)
    {
    	//Aborts event creation if wrong type is given
    	if(type == unknown || type == last_entry)
    	{
    		std::cout << "ERROR: Trying to generate invalid event, discarding." << std::endl;
    		return;
    	}
    	
    	SetEventData(type, 0.f, args...);
    }
	
	template <typename... Payload>
	void EventManager::GenerateEventWithDelay(EventType type, const float& delay, const Payload&... args)
    {
    	//Aborts event creation if wrong type is given
    	if(type == unknown || type == last_entry)
    	{
    		std::cout << "ERROR: Trying to generate invalid event, discarding." << std::endl;
    		return;
    	}
    	
    	SetEventData(type, delay, args...);
    }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// PRIVATE
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Filtered Subscription Handlers ----------------------------------------------------------------------------------

	template <typename T, typename... Payload>
	void EventManager::AddReceiver(const bool& priority, EventReceiver* receiver, const T& type, const Payload&... types)
    {
    	//Throws a compiler error if the wrong type is passed
    	static_assert(std::is_same<T, EventType>::value, "Tried Subscribing invalid event type");

    	// If receiver has priority is moved inside priority list
    	(priority ? priority_event_receiver_list : event_receiver_list).emplace_back(type, receiver);
	
    	//Keeps iterating until every type provided has been subscribed
    	if constexpr (sizeof...(types) > 0)
    	{
    		AddReceiver(priority, receiver, types...);
    	}
    	else
    	{
    		//When iteration ends adds a single instance to ui_receiver_list
    		(priority ? priority_ui_receiver_list : ui_receiver_list).emplace_back(receiver);
    	}
    }

	template <typename T, typename... Payload>
	void EventManager::RemoveReceiver(EventReceiver* receiver, const T& type, const Payload&... types)
    {
    	// Cycles trough all priority receivers backwards
    	for (int i = priority_event_receiver_list.size() -1; i >= 0; --i)
    	{
    		// Removes the element that matches receiver and type provided
    		if(priority_event_receiver_list[i].second == receiver && priority_event_receiver_list[i].first == type)
    		{
    			priority_event_receiver_list.erase(std::remove(priority_event_receiver_list.begin(), priority_event_receiver_list.end(),
					priority_event_receiver_list[i]), priority_event_receiver_list.end());
    		}
    	}
    	
    	// Cycles trough all normal receivers backwards
    	for (int i = event_receiver_list.size() -1; i >= 0; --i)
    	{
    		if(event_receiver_list[i].second == receiver && event_receiver_list[i].first == type)
    		{
    			event_receiver_list.erase(std::remove(event_receiver_list.begin(), event_receiver_list.end(),
					event_receiver_list[i]), event_receiver_list.end());
    		}
    	}
		
    	//Keeps iterating until every type provided has been subscribed
    	if constexpr (sizeof...(types) > 0)
    	{
    		RemoveReceiver(receiver, types...);
    	}
    	else
    	{
    		//When iteration ends unsubscribes the receiver from the list of ui_receivers

    		//Priority
    		priority_ui_receiver_list.erase(std::remove(priority_ui_receiver_list.begin(),
				priority_ui_receiver_list.end(), receiver), priority_ui_receiver_list.end());
    		//Normal
    		ui_receiver_list.erase(std::remove(ui_receiver_list.begin(),
				ui_receiver_list.end(), receiver), ui_receiver_list.end());
    	}
    }
	
    // Event Data Insertion --------------------------------------------------------------------------------------------

	//The following two functions are closely linked together and provide an
	//easy way to bulk data inside events
	
	template <typename... Payload>
	void EventManager::SetEventData(const EventType& type, const float& delay, const Payload&... args)
	{
    	// Creates the event with data given, and an offset to reach the union
    	event_list.emplace_back(type, delay);
		size_t byte_offset = sizeof(Event::type) + sizeof(Event::delay);
		
    	// Starts the population process
    	try
    	{
    		SetEventDataIterative(event_list.back(), byte_offset, args...);
    	}
    	catch (const std::length_error& error)
    	{
    		std::cout << "Caught exception: " << error.what() << std::endl;
    	}
	}

	//Recursive, linked to the previous one
	template <typename T, typename ... Payload>
	void EventManager::SetEventDataIterative(Event& event, size_t& byte_offset, const T& arg, const Payload&... args)
	{
    	//if the memory size of an event has been overflow, stop data insertion
		if(byte_offset + sizeof(arg) > sizeof(Event))
		{
			//If memory is about to be overflown throws error and stops data insertion
			throw std::length_error("Size of data provided exceeds Event size");
		}
		
    	//Does a constexpr to see if the value to be inserted is a char of any type
    	if constexpr (std::is_same<typename std::remove_cv<typename std::remove_extent<T>::type>::type, char>::value)
		{
			//Checks for maximum allowed string size
    		const size_t size = sizeof(Event::SoundEventStart::filename);

    		//Copies the string over to the event but cuts the part that does not fit
    		strncpy_s(*(char(*)[sizeof(T)])((char*)&event + byte_offset), arg, size - 1);

    		//Increases the byte offset by the size of the char array declaration
    		byte_offset += size;
		}
		else
		{
			//If not a char array, cast the memory location offset by the byte_offset
			//as a pointer of the type to be inserted, then move data inside
			*(T*)((char*)&event + byte_offset) = arg;

			if constexpr (sizeof(T) < static_cast<size_t>(4))
			{
				//A boolean is a single byte, to prevent memory shifting and undefined
				//behaviour, offsets the byte offset by 4 byes, a memory cell size
				byte_offset += static_cast<size_t>(4);
			}
			else
			{
				//Offsets the offset by the type size
				byte_offset += sizeof(arg);
			}
		}

		//Keep recurring until all the value in the data package are used
		if constexpr (sizeof...(args) > 0)
		{
			SetEventDataIterative(event, byte_offset, args...);
		}
	}
}

