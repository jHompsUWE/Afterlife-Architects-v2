#include "pch.h"
#include "InputManager.h"

#include "DataManager.h"

namespace AL
{
	InputManager::InputManager() : event_manager(EventManager::Get()), game_data(DataManager::GetGD()) {}

	InputManager::~InputManager() = default;

	void InputManager::init()
	{
		cursor = std::make_unique<ImageGO2D>("Cursor", DataManager::GetD3DDevice());
		cursor->set_layer_depth(128);
		cursor->SetOrigin(Vector2{ 0,0 });
		cursor->SetScale(0.8f);
		cursor_offset = cursor->GetRes();
	}

	// Input Polling ---------------------------------------------------------------------------------------------------

	void InputManager::PollKeyboard(Keyboard::State keyboard)
	{
		//Do not map key-bindings if controller is being used 
		if (controller_active) return;

#ifdef _ARCADE
		//Key Bindings for the arcade machine

		//Camera
		MapEntryToEvent(keyboard.R, Input::camera_up, true);
		MapEntryToEvent(keyboard.F, Input::camera_down, true);
		MapEntryToEvent(keyboard.D, Input::camera_left, true);
		MapEntryToEvent(keyboard.G, Input::camera_right, true);

		//Zoom
		MapEntryToEvent(keyboard.A, Cursor::Action::scroll_up);
		MapEntryToEvent(keyboard.W, Cursor::Action::scroll_down);

		//Movement of cursor
		if (keyboard.Up) cursor_pos.y -= speed;
		if (keyboard.Down) cursor_pos.y += speed;
		if (keyboard.Left) cursor_pos.x -= speed;
		if (keyboard.Right) cursor_pos.x += speed;

		//Cursor click
		MapEntryToEvent(keyboard.LeftShift, Cursor::button_input1);

#else
		//Camera
		MapEntryToEvent(keyboard.W, Input::camera_up, true);
		MapEntryToEvent(keyboard.S, Input::camera_down, true);
		MapEntryToEvent(keyboard.A, Input::camera_left, true);
		MapEntryToEvent(keyboard.D, Input::camera_right, true);
#endif
	}

	void InputManager::PollMouse(Mouse::State mouse)
	{
#ifndef _ARCADE
		//Creates event for mouse scroll if necessary
		MouseScrollToEvent(mouse);

		//Maps mouse_pos to the mouse state pos
		mouse_pos = { mouse.x, mouse.y };

		//Do not map keybindings if controller is being used 
		if (controller_active) return;

		//Map mouse keys here
		MapEntryToEvent(mouse.leftButton, Cursor::button_input1);
		MapEntryToEvent(mouse.rightButton, Cursor::button_input2);
		MapEntryToEvent(mouse.middleButton, Cursor::button_input3);
#endif
	}

	void InputManager::PollGamepad(GamePad::State gamepad)
	{
		if (gamepad.IsConnected())
		{
			//Gathers all the input axis from the first controller connected

			//Comtroller Thumbs-ticks int value 0 - 100
			Vector2Int thumb_l = { static_cast<int>(gamepad.thumbSticks.leftX * 100),
								  static_cast<int>(gamepad.thumbSticks.leftY * 100) };
			//Applies a dead-zone
			thumb_l.x = thumb_l.x > dead_zone || thumb_l.x < -dead_zone ? thumb_l.x : 0;
			thumb_l.y = thumb_l.y > dead_zone || thumb_l.y < -dead_zone ? thumb_l.y : 0;

			Vector2Int thumb_r = { static_cast<int>(gamepad.thumbSticks.rightX * 100),
								  static_cast<int>(gamepad.thumbSticks.rightY * 100) };
			//Applies a dead-zone
			thumb_r.x = thumb_r.x > dead_zone || thumb_r.x < -dead_zone ? thumb_r.x : 0;
			thumb_r.y = thumb_r.y > dead_zone || thumb_r.y < -dead_zone ? thumb_r.y : 0;

			//Controller triggers int value 0 - 100
			Vector2Int triggers = { static_cast<int>(gamepad.triggers.left * 100),
								   static_cast<int>(gamepad.triggers.right * 100) };
			//Applies a dead-zone
			triggers.x = triggers.x > dead_zone || triggers.x < -dead_zone ? triggers.x : 0;
			triggers.y = triggers.y > dead_zone || triggers.y < -dead_zone ? triggers.y : 0;

			//Left thumbstick axis are mapped to cursor movement
			controller_pos.x += thumb_l.x * cursor_speed;
			controller_pos.y -= thumb_l.y * cursor_speed;

			//Right thumbstick axis are mapped to camera movement
			if (thumb_r.x != 0 || thumb_r.y != 0)
			{
				//Sends the Y and X axis of thumbstick right to the camera when
				//they are not 0
				event_manager.GenerateEvent(event_camera, thumb_r.x, thumb_r.y);
			}

			//Do not map specific controller keybindings if mouse is active
			if (!controller_active) return;

			//Cursor interaction 
			if (triggers.x > 0.8f || triggers.y > 0.8f)
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

	void InputManager::UpdateCursorPos(int window_width, int window_height)
	{
		//Offsets resolution
		window_width -= static_cast<int>(cursor_offset.x);
		window_height -= static_cast<int>(cursor_offset.y);

		//Check if mouse position has changed
		if (prev_mouse_pos != mouse_pos)
		{
			prev_mouse_pos = mouse_pos;
			cursor_pos = mouse_pos;

			controller_active = false;
		}

		//Chech if the controller pointer has been moved
		if (controller_pos != prev_controller_pos)
		{
			prev_controller_pos = controller_pos;
			cursor_pos = controller_pos;

			controller_active = true;
		}

		//If the cursor pos has changed, send an event
		if (cursor_pos != prev_cursor_pos)
		{
			//Caps cursor inside the X axis of the screen
			if (cursor_pos.x > window_width) cursor_pos.x = window_width;
			else if (cursor_pos.x < 0) cursor_pos.x = 0;

			//Caps the cursor inside the Y axis of the screen 
			if (cursor_pos.y > window_height) cursor_pos.y = window_height;
			else if (cursor_pos.y < 0) cursor_pos.y = 0;

			prev_cursor_pos = cursor_pos;

			//Syncs the mouse input to 
			controller_pos = cursor_pos;
			prev_controller_pos = cursor_pos;

			//Generate Event accordingly
			event_manager.GenerateEvent(event_cursor_move, cursor_pos.x, cursor_pos.y);
		}

		//Sets new cursor pos
		cursor->SetPos(Vector2(static_cast<float>(cursor_pos.x), static_cast<float>(cursor_pos.y)));
		cursor->Tick(game_data);
	}

	// Key Mapping Handlers --------------------------------------------------------------------------------------------

	void InputManager::MouseScrollToEvent(const Mouse::State& mouse)
	{
		//Gets current scroll value
		const int current_scroll = mouse.scrollWheelValue;

		//Has the scroll value changed?
		if (current_scroll != mouse_scroll)
		{
			if (current_scroll > mouse_scroll)
			{
				event_manager.GenerateEvent(event_cursor_interact, Cursor::Action::scroll_up);
			}
			else if (current_scroll < mouse_scroll)
			{
				event_manager.GenerateEvent(event_cursor_interact, Cursor::Action::scroll_down);
			}
			//Save the new scroll value 
			mouse_scroll = current_scroll;
		}
	}

	void InputManager::MapEntryToEvent(bool state, Input::Action action, bool repeat)
	{
		//If the input is set as repeat, generate an input event each poll action while
		//the button is being pressed
		if (repeat)
		{
			if (state)
			{
				//Repeat event
				event_manager.GenerateEvent(event_input, action, state);
			}
			return;
		}

		//Maps the current states of the outputs to keep track of pressed and released actions
		if (state)
		{
			if (!input_to_action_map[action])
			{
				input_to_action_map[action] = true;
				//Button is pressed
				event_manager.GenerateEvent(event_input, action, state);

			}
		}
		else
		{
			if (input_to_action_map[action])
			{
				input_to_action_map[action] = false;
				//Button is released 
				event_manager.GenerateEvent(event_input, action, state);
			}
		}
	}

	void InputManager::MapEntryToEvent(bool state, Cursor::Action action, bool repeat)
	{
		//If the input is set as repeat, generate an input event each poll action while
		//the button is being pressed
		if (repeat)
		{
			if (state)
			{
				//Repeat Event
				event_manager.GenerateEvent(event_cursor_interact, action, state);
			}
			return;
		}

		//Maps the current states of the outputs to keep track of pressed and relesed actions
		if (state)
		{
			if (!cursor_to_action_map[action])
			{
				cursor_to_action_map[action] = true;
				//Button is pressed 
				event_manager.GenerateEvent(event_cursor_interact, action, state);
			}
		}
		else
		{
			if (cursor_to_action_map[action])
			{
				cursor_to_action_map[action] = false;
				//Button is released 
				event_manager.GenerateEvent(event_cursor_interact, action, state);
			}
		}
	}

	//Cursor Getters ---------------------------------------------------------------------------------------------------

	void InputManager::ResizeWithWindow(const int& window_width, const int& window_height)
	{
		const auto& scale = cursor->ReSize(window_width, window_height);
		cursor_speed *= scale.x;
	}

	ImageGO2D* InputManager::GetCursor() const
	{
		return &*cursor;
	}
}
