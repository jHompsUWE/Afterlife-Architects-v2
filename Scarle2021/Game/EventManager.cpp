#include "pch.h"
#include "EventManager.h"
#include "DataManager.h"

namespace AL
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// PUBLIC 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Default private constructor, no need to initialize inherited class
	EventManager::EventManager() = default;
	
	//Default private de-constructor
	EventManager::~EventManager() = default;

	// Instance --------------------------------------------------------------------------------------------------------
	
	//Gets singleton instance
	EventManager& EventManager::Get()
	{
		static EventManager instance;
		return instance;
	}
	
	// Static ----------------------------------------------------------------------------------------------------------
	
	void EventManager::AddEventReceiver(const bool& priority, EventReceiver* receiver)
	{
		Get().AddReceiver(priority, receiver);
	}
	
	void EventManager::RemoveEventReceiver(EventReceiver* receiver)
	{
		Get().RemoveReceiver(receiver);
	}
	
	void EventManager::IncreaseReceiverPrioritySt(EventReceiver* receiver)
	{
		Get().IncreaseReceiverPriority(receiver);
	}

	void EventManager::DecreaseReceiverPrioritySt(EventReceiver* receiver)
	{
		Get().DecreaseReceiverPriority(receiver);
	}

	std::vector<Event>& EventManager::GetEventListSt()
	{
		return Get().event_list;
	}
	
	void EventManager::FlushEventListSt()
	{
		Get().event_list.clear();
	}

	const bool& EventManager::IsCursorInsideUi()
	{
		return Get().inside_ui;
	}

	// Update ----------------------------------------------------------------------------------------------------------

	void EventManager::LateUpdate()
	{
		//Every update checks if the cursor is present inside any UI element
		inside_ui = false;

		//Prioritized receivers
		for (const auto& priority_receiver : priority_ui_receiver_list)
		{
			//Stop iteration if cursor is inside of even a single one as there is no need to continue
			if(priority_receiver->IsCursorInsideWindow())
			{
				inside_ui = true;
				return;
			}
		}

		//Normal Receivers
		for (const auto& receiver : ui_receiver_list)
		{
			if(receiver->IsCursorInsideWindow())
			{
				inside_ui = true;
				return;
			}
		}
	}
	
	// Event Dispatching -----------------------------------------------------------------------------------------------
	
	void EventManager::DispatchEvents()
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
				//Keeping track if the event has been consumed
				bool consumed = false;
				
				//Events are dispatched to the priority receivers first
				for (const auto& priority_receiver : priority_event_receiver_list)
				{
					//Fires the event only if the receiver matches the type it is listening for 
					if(priority_receiver.first == current_ev.type)
					{
						if(priority_receiver.second->ReceiveEvents(current_ev))
						{
							consumed = true;
							break;
						}
					}
				}

				//Does not dispatch to non-priority receivers if the event has been consumed by priority receivers
				if(!consumed)
				{
					//Event is then dispatched to non-priority receivers
					for (const auto& receiver : event_receiver_list)
					{
						//Fires the event only if the receiver matches the type it is listening for 
						if(receiver.first == current_ev.type)
						{
							if(receiver.second->ReceiveEvents(current_ev)) break;
						}
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

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// PRIVATE
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Unfiltered Subscription Handlers --------------------------------------------------------------------------------
	
	void EventManager::AddReceiver(const bool& priority, EventReceiver* receiver)
	{
		//Subscribe a receiver to every event type
		for (int i = last_entry; i > 0; --i)
		{
			(priority ? priority_event_receiver_list : event_receiver_list).emplace_back(static_cast<EventType>(i), receiver);
		}

		//Subscribe only one instance of a receiver to a UI type
		(priority ? priority_ui_receiver_list : ui_receiver_list).emplace_back(receiver);
	}

	void EventManager::RemoveReceiver(EventReceiver* receiver)
	{
		//Unsubscribes every entry of receiver all receivers vectors

		//Priority receiver list
		for (int i = priority_event_receiver_list.size() -1; i >= 0; --i)
		{
			// Checks each entry to match the receiver ptr, if true removes that specific entry
			if(priority_event_receiver_list[i].second == receiver)
			{
				priority_event_receiver_list.erase(std::remove(priority_event_receiver_list.begin(),
					priority_event_receiver_list.end(), priority_event_receiver_list[i]), priority_event_receiver_list.end());
			}
		}
		
		//Event receiver list
		for (int i = event_receiver_list.size() -1; i >= 0; --i)
		{
			// Checks each entry to match the receiver ptr, if true removes that specific entry
			if(event_receiver_list[i].second == receiver)
			{
				event_receiver_list.erase(std::remove(event_receiver_list.begin(),
					event_receiver_list.end(), event_receiver_list[i]), event_receiver_list.end());
			}
		}

		//UI receiver list
		
		//Priority
		priority_ui_receiver_list.erase(std::remove(priority_ui_receiver_list.begin(),
			priority_ui_receiver_list.end(), receiver), priority_ui_receiver_list.end());
		//Normal
		ui_receiver_list.erase(std::remove(ui_receiver_list.begin(),
			ui_receiver_list.end(), receiver), ui_receiver_list.end());
	}

	// Receiver Priority Handlers --------------------------------------------------------------------------------------
	
	void EventManager::IncreaseReceiverPriority(EventReceiver* receiver)
	{
		//Moves all receivers matching the current receiver to the front of both vectors
		//Uses stable partition to move all elements back by one and replacing the desired at the front

		//All receivers
		auto partition_point_ev =
			std::stable_partition(event_receiver_list.begin(), event_receiver_list.end(),
			[receiver](const std::pair<EventType, EventReceiver*>& element)
			{
				return element.second == receiver;
			});

		//Ui receivers
		auto partition_point_ui =
			std::stable_partition(ui_receiver_list.begin(), ui_receiver_list.end(),
			[receiver](const EventReceiver* element)
			{
				return element == receiver;
			});
	}
	
	void EventManager::DecreaseReceiverPriority(EventReceiver* receiver)
	{
		//Moves all receivers matching the current receiver to the back of both vectors
		//Uses stable partition to move all elements forward by one and replacing the desired at the back

		//All receivers
		auto partition_point_ev =
			std::stable_partition(event_receiver_list.begin(), event_receiver_list.end(),
			[receiver](const std::pair<EventType, EventReceiver*>& element)
			{
				return element.second != receiver;
			});

		//Ui receivers
		auto partition_point_ui =
			std::stable_partition(ui_receiver_list.begin(), ui_receiver_list.end(),
			[receiver](const EventReceiver* element)
			{
				return element != receiver;
			});
	}
}
