#pragma once
#include "Event.hpp"

namespace AL
{
	/**
	 * \brief Observer design pattern for classes that
	 * subscribe to receive Events
	 */
	class EventReceiver
	{
	public:
		virtual ~EventReceiver() = default;

		/**
		 * \brief Connection function to the Event Manager
		 */
		virtual bool ReceiveEvents(const Event& al_event) = 0;

		/**
		 * \brief to-be overridden function to check if the mouse pointer is present inside the UI
		 * \return Returns if the mouse pointer is inside a window UI
		 */
		virtual bool IsCursorInsideWindow()
		{
			return false;
		}
	};
}


