#pragma once

#include "Event.hpp"
#include "EventReceiver.hpp"

#include <unordered_map>

namespace AL
{
	class EventManager
	{
	public:
		//Deleted copy constructor and assignment operator
		EventManager(const EventManager&) = delete;
		EventManager& operator=(const EventManager&) = delete;

		/**
		 * \return Instance of EventManager
		 */
		static EventManager& Get();

		// Static Event Generation -------------------------------------------------------------------------------------
		
		/**
		 * \brief Static function to generate an event
         * \tparam Payload Data to be inserted in the event
         * \param type which type of even this is gonna be
         */
		template <typename... Payload>
		static void GenerateEventSt(const EventType& type, const Payload&... args);

		/**
		 * \brief Static function to generate an event with a delay
		 * \tparam Payload Data to be inserted in the event
		 * \param type which type of even this is gonna be
		 * \param delay delay in seconds before an event is fired
		 */
		template <typename... Payload>
		static void GenerateEventWithDelaySt(const EventType& type, const float& delay, const Payload&... args);

		// Static Filtered Subscriptions handlers ----------------------------------------------------------------------
		
		/**
		 * \brief Subscribe a receiver to listen for specific events
		 * \tparam Payload Event Types to listen to
		 * \param priority Event will be added to the priority queue
		 * \param receiver ptr to the receiver 
		 * \param types list of types
		*/
		template <typename... Payload>
		static void AddEventReceiver(const bool& priority, EventReceiver* receiver, const Payload&... types);

		/**
		 * \brief Unsubscribes a receiver from listening to specific events
		 * \tparam Payload Event Typs to unsubscribe from
		 * \param receiver ptr to the receiver 
		 * \param types list of types
		 */
		template <typename... Payload>
		static void RemoveEventReceiver(EventReceiver* receiver, const Payload&... types);

		// Static Unfiltered Subscriptions handlers --------------------------------------------------------------------
		
		/**
		 * \param priority if it will be inserted in the priority list
		 * \param receiver to be subscribed to receive events
		 */
		static void AddEventReceiver(const bool& priority, EventReceiver* receiver);

		/**
	     * \param receiver to be unsubscribed from receiving events
	     */
		static void RemoveEventReceiver(EventReceiver* receiver);

		// Static priority system --------------------------------------------------------------------------------------
		
		/**
		 * \brief Increases the priority of which events are dispatched to this current receiver
		 * \param receiver Pointer to the receiver
		 */
		static void IncreaseReceiverPrioritySt(EventReceiver* receiver);

		/**
		 * \brief Decreases the priority of which events are dispatched to this current receiver
		 * \param receiver Pointer to the receiver
		 */
		static void DecreaseReceiverPrioritySt(EventReceiver* receiver);

		// Static Getters/Setters --------------------------------------------------------------------------------------
		
		/**
		 * \return static func to return the event list
		 */
		static std::vector<Event>& GetEventListSt();

		/**
		 * \brief Flushes the event list
		 */
		static void FlushEventListSt();

		/**
		 * \return true if the cursor is inside a UI window
		 */
		static const bool& IsCursorInsideUi();

		// Public ------------------------------------------------------------------------------------------------------

		/**
		 * \brief Runs the update function of the EV
		 */
		void LateUpdate();
		
		// Event Dispatching -------------------------------------------------------------------------------------------

		/**
		 * \brief Routes the events to each of the receivers
		 * Type check is done to provide the events receivers have only subscribed for 
		 */
		void DispatchEvents();

		// Event Generation --------------------------------------------------------------------------------------------

		/**
    	 * \brief Generates an event form the data given, no delay before is fired
    	 * \tparam Payload Data to be inserted in the event
     	 * \param type which type of even this is gonna be
    	 */
		template <typename... Payload>
		void GenerateEvent(EventType type, const Payload&... args);

		/**
		 * \brief Generates an event form the data given, delay is applied
		 * \tparam Payload Data to be inserted in the event
		  * \param type which type of even this is gonna be
		  * \param delay amount of delay before the event is fired, in seconds
		 */
		template <typename... Payload>
		void GenerateEventWithDelay(EventType type, const float& delay, const Payload&... args);
		
	private:
		//Private constructor and de-constructor
		EventManager();
		~EventManager();

		// Unfiltered Subscription Handlers ----------------------------------------------------------------------------
	
		/**
		 * \brief Subscribes the provided receiver to every event type available
		 * \param priority true if the receiver has to be put in the priority list
		 * \param receiver Pointer to the receiver to subscribe
		 */
		void AddReceiver(const bool& priority, EventReceiver* receiver);

		/**
		 * \brief Fully unsubscribes a receiver from listening to any event
		 * \param receiver Pointer to the receiver to unsubscribe
		 */
		void RemoveReceiver(EventReceiver* receiver);

		// Filtered Subscription Handlers ------------------------------------------------------------------------------

		/**
		 * \brief 
		 * \tparam T first type to subscribe the current receiver to
		 * \tparam Payload Rest of types to subscribe the current receiver to
		 * \param priority Event will be added to the priority queue
		 * \param receiver prt to the receiver to add
		 * \param type first type 
		 * \param types rest of types
		 */
		template <typename T, typename... Payload>
		void AddReceiver(const bool& priority, EventReceiver* receiver, const T& type, const Payload&... types);
	
		/**
		 * \brief 
		 * \tparam T first type to unsubscribe the current receiver from
		 * \tparam Payload Rest of types to unsubscribe the current receiver from
		 * \param receiver prt to the receiver to unsubscribe
		 * \param type first type 
		 * \param types rest of types
		 */
		template <typename T, typename... Payload>
		void RemoveReceiver(EventReceiver* receiver, const T& type, const Payload&... types);

		// Receiver Priority Handlers ----------------------------------------------------------------------------------

		/**
		 * \brief Moves the specified receiver at the front of the receivers queue
		 * \param receiver Pointer to the receiver
		 */
		void IncreaseReceiverPriority(EventReceiver* receiver);

		/**
		 * \brief Moves the specified receiver at the back of the receivers queue
		 * \param receiver Pointer to the receiver
		 */
		void DecreaseReceiverPriority(EventReceiver* receiver);
		
		// Event Data Insertion ----------------------------------------------------------------------------------------
		
		/**
		 * \brief This function takes and data, will the store all the data provided inside the event.
		 * \tparam Payload variadic template, this is the packet of values and types that need to be inserted
		 * \param type type of the event to be generated
		 * \param delay before the event is fired
		 */
		template<typename... Payload>
		void SetEventData(const EventType& type, const float& delay, const Payload&... args);

		/**
    	 * \brief This is closely linked to the previous function and will keep recurring until all the data has been moved
    	 * \tparam T type of the current type that is being inserted
    	 * \tparam Payload packet or remaining data 
    	 * \param event reference to the event
    	 * \param byte_offset offset of bytes from the initial memory address 
    	 */
		template<typename T, typename... Payload>
		void SetEventDataIterative(Event& event, size_t& byte_offset, const T& arg, const Payload&... args);

		// Data --------------------------------------------------------------------------------------------------------

		std::vector<std::pair<EventType, EventReceiver*>> priority_event_receiver_list{};
		std::vector<std::pair<EventType, EventReceiver*>> event_receiver_list{};
		std::vector<EventReceiver*> priority_ui_receiver_list{};
		std::vector<EventReceiver*> ui_receiver_list{};
		std::vector<Event> event_list{};

		bool inside_ui = false;
	};
}

// A TPP file for templates is added at the bottom of this H file to allow the compiler to link it properly
#include "EventManager.tpp"
