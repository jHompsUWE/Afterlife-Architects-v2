#pragma once
#include <vector>

#include "DrawData2D.h"
#include "GameData.h"
#include "SimpleMath.h"
#include "UIWindow.h"


class HierarchyManager : public AL::EventReceiver
{
public:
	HierarchyManager();
	~HierarchyManager() override;

	// Game Cycle ------------------------------------------------------------------------------------------------------
	
	/**
	 * \brief Update the windows added to the hierarchy
	 * \param game_data prt to game_data
	 * \param mouse_position Vector2 coordinates of mouse position
	 */
	void Update(GameData* game_data, Vector2& mouse_position) const;

	/**
	 * \brief Renders the windows added to the hierarchy
	 * \param draw_data 
	 */
	void Render(DrawData2D* draw_data) const;

	// Hierarchy subscription/unsubscription ---------------------------------------------------------------------------
	
	/**
	 * \brief Adds to the Hierarchy the window provided
	 * \param window Ptr to a derivative class of UIWindow
	 */
	void AddToHierarchy(UIWindow* window);

	/**
	 * \brief Removes from the Hierarchy the window provided
	 * \param window Ptr to a derivative class of UIWindow
	 */
	void RemoveFromHierarchy(UIWindow* window);

	// Open / Close ----------------------------------------------------------------------------------------------------
	
	/**
	 * \brief Toggles open/close of the provided window, in addition brings it on top when opened
	 * \param window Prt to the window
	 */
	void OpenCloseWindow(UIWindow* window);

	// Layering --------------------------------------------------------------------------------------------------------
	
	/**
	 * \brief Moves the provided window on top of the hierarchy 
	 * \param window Ptr to the window
	 */
	void MoveInFront(UIWindow* window);

	/**
	 * \brief Bulk moves all the windows to the top of the hierarchy
	 * This is needed to shift windows forward in the receiver vector
	 */
	void MoveAllToFront() const;

	/**
	 * \brief When called, brings the window clicked with the mouse at the top of the hierarchy 
	 */
	void MoveToFrontIfInside();

	// Extra -----------------------------------------------------------------------------------------------------------
	
	/**
	 * \brief Runs the ReSize function to all the windows added to the hierarchy
	 */
	void ResizeAll() const;
	
	//Overriden from Event Receiver
	bool ReceiveEvents(const AL::Event& al_event) override;
	
private:
	//Elements are stored inside this vector in the order wanted in the heirarchy
	std::vector<UIWindow*> windows {};
	Vector2 mouse_pos {0,0};
};
