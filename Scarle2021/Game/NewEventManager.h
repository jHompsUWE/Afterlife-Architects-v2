#pragma once

#include "gamedata.h"
#include "Event.h"
#include "Observable.h"
#include "Observer.h"
#include "Packet.h"
#include "Vector2Int.h"

#include <unordered_map>
#include <SimpleMath.h>

namespace AL
{
	class NewEventManager : public IObservable<IEventReceiver>
	{
	public:
		//Deleted copy constructor and assignment operator
		NewEventManager(const NewEventManager&) = delete;
		NewEventManager& operator=(const NewEventManager&) = delete;

		//Gets singleton instance
		static NewEventManager& Get();
		
		//Static public functions, accessible via ::
		template <typename... Payload>
		static void GenerateEventSt(EventType type, const Payload&... args);
		static std::vector<Event>& GetEventListSt();
		static void FlushEventListSt();
		static void AddEventReceiver(IEventReceiver* observer);
		static void RemoveEventReceiver(IEventReceiver* observer);

		//Public functions, accessible via Get() or via singleton instance
		std::vector<Event>& GetEventList();
		void FlushEventList();
		//Data sharing
		void BroadcastData() override;

		//Input polling
		void PollKeyboard(Keyboard::State keyboard);
		void PollMouse(Mouse::State mouse);
		void PollGamepad(GamePad::State gamepad, const float& dt);
		void UpdateCursorPos(const int& window_width, const int& window_height);

		//Button Work Around
		void GenerateEventSoundStart(const char filename[32], const float& volume, const bool& loop);
		void GenerateEventSoundStop(const char filename[32]);
		void GenerateInterfaceEvent(const UI::Action& action);
		void GenerateBuildSysEvent(const BuildSys::Section& section, const StructureType& structure, const ZoneType& zone);
		void GenerateGameEvent(const Game::Action& action);

		//Generate Events
		template <typename... Payload>
		void GenerateEvent(EventType type, const Payload&... args);

		//Getter for cursor sprite
		SimpleMath::Vector2 GetCursorPos() const;
		//Setter for sprite speed
		void SetSpriteSpeed(const SimpleMath::Vector2& new_speed);
		
	private:
		//Private constructor and de-constructor
		NewEventManager();
		~NewEventManager() override;

		//Key mapping
		void MouseScrollToEvent(const Mouse::State& mouse);
		void MapEntryToEvent(bool state, Input::Action action, bool repeat = false);
		void MapEntryToEvent(bool state, Cursor::Action action, bool repeat = false);

		//The following two functions are closely linked together and provide an
		//easy way to bulk data inside events
		template<typename... Payload>
		void SetEventData(Event& event, const Payload&... args);
		template<typename T, typename... Payload>
		void SetEventData(Event& event, int& byte_offset, const T& arg, const Payload&... args);
		
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

