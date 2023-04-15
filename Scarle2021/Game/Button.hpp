#pragma once
#include <iostream>
#include <ostream>

#include "DataManager.h"
#include "UIButtonInterFace.h"

template<typename Action1, typename Action2>
class Button : public UIButtonInterFace, public AL::EventReceiver
{
public:
	Button(Vector2 _buttonPosition, ID3D11Device* _d3dDevice,std::string
	_text,std::string _filepath,AL::EventType _event, Action1 _action1, Action2 _action2 ,Vector2 _setScale)
		: UIButtonInterFace(_event), action_1(_action1), action_2(_action2)
	{
		//setup for button background
		buttonBackGround = new ImageGO2D(_filepath, _d3dDevice);
		buttonBackGround->SetOrigin(Vector2(0, 0));
		//buttonBackGround->SetScale(Vector2(_setScale));

		//sets res
		button_res = Vector2(buttonBackGround->GetRes().x
			* buttonBackGround->GetScale().x, buttonBackGround->GetRes().y
			* buttonBackGround->GetScale().y);
	
		button_pos = _buttonPosition - button_res/2;
		buttonBackGround->SetPos(button_pos);

		//setup button text
		buttonText = new TextGO2D(_text);
		buttonText->SetColour(Color((float*)&Colors::Black));
		buttonText->SetPos(Vector2(button_pos.x,button_pos.y));
		buttonText->SetScale(Vector2(_setScale));

		AL::NewEventManager::AddEventReceiver(this, AL::EventType::event_cursor_move, AL::EventType::event_cursor_interact);
	};
	
	Button(Vector2 _buttonPosition, ID3D11Device* _d3dDevice,std::string
	_filepath,AL::EventType _event, Action1 _action1, Action2 _action2, Vector2 _setScale)
		: UIButtonInterFace(_event), action_1(_action1), action_2(_action2)
	{
		//setup for button background
		buttonBackGround = new ImageGO2D(_filepath, _d3dDevice);
		buttonBackGround->SetOrigin(Vector2(0, 0));
		buttonBackGround->SetScale(_setScale);
		//sets res
		button_res = Vector2(buttonBackGround->GetRes().x
			* buttonBackGround->GetScale().x, buttonBackGround->GetRes().y
			* buttonBackGround->GetScale().y);
	
		button_pos = _buttonPosition - button_res/2;
		buttonBackGround->SetPos(button_pos);

		AL::NewEventManager::AddEventReceiver(this, AL::EventType::event_cursor_move, AL::EventType::event_cursor_interact);
	}
	
	~Button() override
	{
		AL::NewEventManager::RemoveEventReceiver(this);
		
		delete buttonBackGround;
		delete buttonText;
	}

	const bool& ReceiveEvents(const AL::Event& al_event) override
	{
		if(!interactable) return false;
		
		switch (al_event.type)
		{
		case AL::event_cursor_move:
			//Only when the cursor is moved update position
			mouse_pos = Vector2{(float)al_event.cursor_moved.pos_x, (float)al_event.cursor_moved.pos_y};
			break;
			
		case AL::event_cursor_interact:

			//Checks if the button specified is being pressed
			if(al_event.cursor_interact.action == AL::Cursor::button_input1)
			{
				//If the button has been pressed, continue
				if(al_event.cursor_interact.active == true)
				{
					//Mouse pos is inside button? carry our action
					if(isInside(mouse_pos))
					{
						close_window = true;

						//Evaluates the correct event to generate, if the second value is int, it means it has to be
						//scrapped and the event will be generated with a single value
						if constexpr (std::is_same<Action2, int>::value)
						{
							AL::NewEventManager::GenerateEventSt(saved_event, action_1);
						}
						//If the first action is from the building system, generate a building event
						else if constexpr (std::is_same<Action1, AL::BuildSys::Section>::value)
						{
							AL::NewEventManager::GenerateEventSt(saved_event, (AL::BuildSys::Section)action_1,
								(StructureType)action_2, (ZoneType)action_2);
						}
						//For any other iteration generate a normal event
						else
						{
							AL::NewEventManager::GenerateEventSt(saved_event, action_1, action_2);
						}

						return true;
					}
				}
			}
			
			break;

		default:
			break;
		}

		return false;
	}

	void update(GameData* _gameData) override
	{
		buttonBackGround->Tick(_gameData);
	
		if(buttonText != nullptr)
		{
			buttonText->Tick(_gameData);
		}
	}
	
	void render(DrawData2D* _drawData) override
	{
		buttonBackGround->Draw(_drawData);
	
		if(buttonText != nullptr)
		{
			buttonText->Draw(_drawData);
		}
	}
	
	void setPostion(Vector2 _new_pos) override
	{
		// WIP
		button_pos = _new_pos;
		buttonBackGround->SetPos(_new_pos);
	
		if(buttonText != nullptr)
		{
			buttonText->SetPos(_new_pos);
		}
	}
	
	
	void setScale(Vector2& _newScale) override
	{
		//_newScale = Vector2(0,0);
	}
	
	Vector2& getPosition() override
	{
		// TODO: insert return statement here
		return button_pos;
	}
	
	Vector2& getButtonRes() override
	{
		// TODO: insert return statement here
		return button_res;
	}
	
	void reSize(Vector2 game_res) override
	{
		//resizes background and text
		auto& resize_scale = buttonBackGround->ReSize(game_res.x, game_res.y);
		if(buttonText != nullptr)
		{
			buttonText->ReSize(game_res.x, game_res.y);
		}

		//Re-scale position and scale accordingly to make button work in UI 
		button_pos = button_pos * resize_scale;
		button_res = button_res * resize_scale;
		
	}

	void MoveInFront() override
	{
		AL::NewEventManager::IncreaseReceiverPrioritySt(this);
	}

	void MoveToBack() override
	{
		AL::NewEventManager::DecreaseReceiverPrioritySt(this);
	}

private:
	//mouse pointer inside button
	bool isInside(Vector2& point) const
	{
		if(point.x >= button_pos.x && point.x <= (button_pos.x + button_res.x) &&
			   point.y >= button_pos.y && point.y <= (button_pos.y + button_res.y))
			   	return true;
    
		return false;
	}

	Vector2 mouse_pos {0,0};
	
	Action1 action_1 = NULL;
	Action2 action_2 = NULL;
};





