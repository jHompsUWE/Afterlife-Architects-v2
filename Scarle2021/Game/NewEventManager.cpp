#include "pch.h"
#include "NewEventManager.h"
#include "DataManager.h"

namespace AL
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// PUBLIC 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
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
	
	void NewEventManager::IncreaseReceiverPrioritySt(EventReceiver* receiver)
	{
		Get().IncreaseReceiverPriority(receiver);
	}

	void NewEventManager::DecreaseReceiverPrioritySt(EventReceiver* receiver)
	{
		Get().DecreaseReceiverPriority(receiver);
	}

	std::vector<Event>& NewEventManager::GetEventListSt()
	{
		return Get().event_list;
	}
	
	void NewEventManager::FlushEventListSt()
	{
		Get().event_list.clear();
	}

	const bool& NewEventManager::IsCursorInsideUi()
	{
		return Get().inside_ui;
	}

	// Update ----------------------------------------------------------------------------------------------------------

	void NewEventManager::LateUpdate()
	{
		//Every update checks if the cursor is present inside any UI element
		inside_ui = false;
		for (const auto& receiver : ui_receiver_list)
		{
			//Stop iteration if cursor is inside of even a single one as there is no need to contiue
			if(receiver->IsCursorInsideWindow())
			{
				inside_ui = true;
				break;
			}
		}
	}
	
	// Event Dispatching -----------------------------------------------------------------------------------------------
	
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
				for (const auto& receiver : event_receiver_list)
				{
					//Fires the event only if the receiver matches the type it is listening for 
					if(receiver.first == current_ev.type)
					{
						if(receiver.second->ReceiveEvents(current_ev)) break;
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
	
	void NewEventManager::AddReceiver(EventReceiver* receiver)
	{
		//Subscribe a receiver to every event type
		for (int i = last_entry; i > 0; --i)
		{
			event_receiver_list.emplace_back(static_cast<EventType>(i), receiver);
		}

		//Subscribe only one instance of a receiver to a UI type
		ui_receiver_list.emplace_back(receiver);
	}

	void NewEventManager::RemoveReceiver(EventReceiver* receiver)
	{
		//Unsubscribes every entry of receiver from both vectors

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
		ui_receiver_list.erase(std::remove(ui_receiver_list.begin(),
			ui_receiver_list.end(), receiver), ui_receiver_list.end());
	}

	// Receiver Priority Handlers --------------------------------------------------------------------------------------
	
	void NewEventManager::IncreaseReceiverPriority(EventReceiver* receiver)
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
	
	void NewEventManager::DecreaseReceiverPriority(EventReceiver* receiver)
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
