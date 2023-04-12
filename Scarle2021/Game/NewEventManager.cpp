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

	// Unfiltered Subscription Handlers --------------------------------------------------------------------------------
	
	void NewEventManager::AddReceiver(EventReceiver* receiver)
	{
		for (int i = last_entry; i > 0; --i)
		{
			receiver_list.emplace_back(static_cast<EventType>(i), receiver);
		}
	}

	void NewEventManager::RemoveReceiver(EventReceiver* receiver)
	{
		for (int i = receiver_list.size() -1; i >= 0; --i)
		{
			// Checks each entry to match the receiver ptr, if true removes that specific entry
			if(receiver_list[i].second == receiver)
			{
				receiver_list.erase(std::remove(receiver_list.begin(),
					receiver_list.end(), receiver_list[i]), receiver_list.end());
			}
		}
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
}
