#pragma once

#include "EventManager.h"
#include "GameData.h"
#include "ImageGO2D.h"
#include "SimpleMath.h"
#include "Vector2Int.h"

namespace AL
{
    class InputManager
    {
    public:
     InputManager();
     ~InputManager();

     void init();

     //Input polling ---------------------------------------------------------------------------------------------------

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
     void PollGamepad(GamePad::State gamepad);

     /**
      * @brief Checks the position of the various pointing devices and generates an event accordingly
      * Based on the last input registered, the input will be automatically set to controller or m&k
      */
     void UpdateCursorPos(int window_width, int window_height);

     //Cursor Getters --------------------------------------------------------------------------------------------------

     /**
      * \brief Resizes the cursor and its speed when the window is resized
      * \param window_width New width of resolution 
      * \param window_height New height of resolution
      */
     void ResizeWithWindow(const int& window_width, const int& window_height);

     /**
      * \brief Return the pointer to the cursor sprite
      * \return Pointer to the cursor 
      */
     ImageGO2D* GetCursor() const;
    
    private:
     // Key Mapping Handlers -------------------------------------------------------------------------------------------

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

     // Data -----------------------------------------------------------------------------------------------------------
     EventManager& event_manager;
     GameData* game_data = nullptr;
     		
     //Key mapping data
     std::unordered_map<Input::Action, bool> input_to_action_map{};
     std::unordered_map<Cursor::Action, bool> cursor_to_action_map{};

     //Cursor
     std::unique_ptr<ImageGO2D> cursor = nullptr;
     Vector2 cursor_offset {0,0};
     
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

#ifdef _ARCADE
     //mouse speed using keys
     float speed = 5.f;
#endif
    };
}

