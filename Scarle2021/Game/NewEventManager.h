#pragma once

#include "gamedata.h"
#include "Event.hpp"
#include "EventDispatcher.h"
#include "EventReceiver.h"
#include "Packet.hpp"
#include "Vector2Int.h"

#include <unordered_map>
#include <SimpleMath.h>

namespace AL
{
	class NewEventManager : public EventDispatcher
	{
	public:
		//Deleted copy constructor and assignment operator
		NewEventManager(const NewEventManager&) = delete;
		NewEventManager& operator=(const NewEventManager&) = delete;

		/**
		 * \return Instance of EventManager
		 */
		static NewEventManager& Get();

		// Static ------------------------------------------------------------------------------------------------------
		
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

		/**
		 * \brief Subscribe a receiver to listen for specific events
		 * \tparam Payload Event Typs to listen to
		 * \param receiver ptr to the receiver 
		 * \param types list of types
		*/
		template <typename... Payload>
		static void AddEventReceiver(EventReceiver* receiver, const Payload&... types);

		/**
		 * \brief Unsubscribes a receiver from listening to specific events
		 * \tparam Payload Event Typs to unsubscribe from
		 * \param receiver ptr to the receiver 
		 * \param types list of types
		 */
		template <typename... Payload>
		static void RemoveEventReceiver(EventReceiver* receiver, const Payload&... types);
		
		/**
		 * \param receiver to be subscribed to receive events
		 */
		static void AddEventReceiver(EventReceiver* receiver);

		/**
	     * \param receiver to be unsubscribed from receiving events
	     */
		static void RemoveEventReceiver(EventReceiver* receiver);
		
		/**
		 * \return static func to return the event list
		 */
		static std::vector<Event>& GetEventListSt();

		/**
		 * \brief Flushes the event list
		 */
		static void FlushEventListSt();

		// Public ------------------------------------------------------------------------------------------------------

		/**
		 * \return returns the event list.
		 */
		std::vector<Event>& GetEventList();

		/**
		 * \brief Clear all data from the event list
		 */
		void FlushEventList();
		
		//Data sharing -------------------------------------------------------------------------------------------------

		/**
		 * \brief Routes the events to each of the receivers
		 * Type check is done to provide the events receivers have only subscribed for 
		 */
		void DispatchEvents() override;

		//Input polling ------------------------------------------------------------------------------------------------

		/**
		 * \brief To map keyboard entries to events
		 * \param keyboard state in this current frame
		 */
		void PollKeyboard(Keyboard::State keyboard);

		/**
		 * \brief to map mouse entries to events
		 * \param mouse state in this current frame
		 */
		void PollMouse(Mouse::State mouse);

		/**
		 * \brief to map gamepad entries to events
		 * \param gamepad state in this current frame
		 */
		void PollGamepad(GamePad::State gamepad, const float& dt);

		/**
		 * @brief Checks the position of the various pointing devices and generates an event accordingly
		 * Based on the last input registered, the input will be automatically set to controller or m&k
		 */
		void UpdateCursorPos(const int& window_width, const int& window_height);

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

		//Cursor Getters -----------------------------------------------------------------------------------------------

		/**
		 * \brief Access the Event Manager to retrieve cursor data
		 * \return Cursor position in pixels
		 */
		SimpleMath::Vector2 GetCursorPos() const;

		/**
		 * \brief Updates the cursor speed with the one provided
		 * \param new_speed Speed on X and Y axis as vector2
		 */
		void SetSpriteSpeed(const SimpleMath::Vector2& new_speed);
		
	private:
		//Private constructor and de-constructor
		NewEventManager();
		~NewEventManager() override;

		// Key Mapping Handlers ----------------------------------------------------------------------------------------

		/**
		 * \brief Generates appropriate event when mouse wheel is scrolled
		 * \param mouse reference to the directX mouse state in use
		 */
		void MouseScrollToEvent(const Mouse::State& mouse);

		/**
		 * \brief Maps a Input State to an action
		 * \param state If the action is happening
		 * \param action What action is happening
		 * \param repeat Should the action be repeated every frame
		 */
		void MapEntryToEvent(bool state, Input::Action action, bool repeat = false);
		
		/**
		 * \brief Maps a Cursor State to an action
		 * \param state If the action is happening
		 * \param action What action is happening
		 * \param repeat Should the action be repeated every frame
		 */
		void MapEntryToEvent(bool state, Cursor::Action action, bool repeat = false);

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
		
		//Key mapping data
		std::unordered_map<Input::Action, bool> input_to_action_map{};
		std::unordered_map<Cursor::Action, bool> cursor_to_action_map{};

		//events
		std::vector<Event> event_list{};

		//Input data
		float cursor_speed = 0.15f;
		int mouse_scroll = 0;
		
		//Position of the cursor on the screen
		Vector2Int prev_cursor_pos {0,0};
		Vector2Int cursor_pos {540,360};

		//Mouse position handling
		Vector2Int prev_mouse_pos {0,0};
		Vector2Int mouse_pos {0,0};

		//Controller position handling
		Vector2Int prev_controller_pos {0,0};
		SimpleMath::Vector2 controller_pos_float {0,0};
		Vector2Int controller_pos {0,0};

		//Keeping track of controller usage
		bool controller_active = false;
		int dead_zone = 0;
	};
}

// A TPP file for templates is added at the bottom of this H file to allow the compiler to link it properly
#include "NewEventManager.tpp"