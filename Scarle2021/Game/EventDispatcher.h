#pragma once
#include <vector>

#include "EventReceiver.h"

namespace AL
{
	/**
	 * \brief Base interface for an observable
	 * Contains the functionality to add and remove observers
	 * Notify will update all the observers 
	 */
	class EventDispatcher
	{
	public:
		virtual ~EventDispatcher() = default;

		// Unfiltered Subscription -----------------------------------------------------------------------------------------
	
		/**
		 * \brief Subscribes the provided receiver to every event type available
		 * \param receiver Pointer to the receiver to subscribe
		 */
		void AddReceiver(EventReceiver* receiver)
		{
			for (int i = last_entry; i > 0; --i)
			{
				receiver_list.emplace_back(static_cast<AL::EventType>(i), receiver);
			}
		}

		/**
		 * \brief Fully unsubscribes a receiver from listening to any event
		 * \param receiver Pointer to the receiver to unsubscribe
		 */
		void RemoveReceiver(EventReceiver* receiver)
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

		// Filtered Subscription -------------------------------------------------------------------------------------------

		/**
		 * \brief 
		 * \tparam T first type to subscribe the current receiver to
		 * \tparam Payload Rest of types to subscribe the current receiver to
		 * \param receiver prt to the receiver to add
		 * \param type first type 
		 * \param types rest of types
		 */
		template <typename T, typename... Payload>
		void AddReceiver(EventReceiver* receiver, const T& type, const Payload&... types)
		{
			//Throws a compiler error if the wrong type is passed
			static_assert(std::is_same<T, EventType>::value, "Tried Subscribing invalid event type");
	
			receiver_list.emplace_back(type, receiver);
	
			//Keeps iterating until every type provided has been subscribed
			if constexpr (sizeof...(types) > 0)
			{
				AddReceiver(receiver, types...);
			}
		}
	
		/**
		 * \brief 
		 * \tparam T first type to unsubscribe the current receiver from
		 * \tparam Payload Rest of types to unsubscribe the current receiver from
		 * \param receiver prt to the receiver to unsubscribe
		 * \param type first type 
		 * \param types rest of types
		 */
		template <typename T, typename... Payload>
		void RemoveReceiver(EventReceiver* receiver, const T& type, const Payload&... types)
		{
			// Cycles trough all receivers backwards
			for (int i = receiver_list.size() -1; i >= 0; --i)
			{
				// Removes the element that matches receiver and type provided
				if(receiver_list[i].second == receiver && receiver_list[i].first == type)
				{
					receiver_list.erase(std::remove(receiver_list.begin(), receiver_list.end(),
						receiver_list[i]), receiver_list.end());
				}
			}
		
			//Keeps iterating until every type provided has been subscribed
			if constexpr (sizeof...(types) > 0)
			{
				RemoveReceiver(receiver, types...);
			}
		}
	
		// BroadCast Data --------------------------------------------------------------------------------------------------
	
		/**
		 * \brief When called, notifies all the receivers
		 */
		virtual void DispatchEvents() = 0;

	protected:
		std::vector<std::pair<EventType, EventReceiver*>> receiver_list{};
	};
}
