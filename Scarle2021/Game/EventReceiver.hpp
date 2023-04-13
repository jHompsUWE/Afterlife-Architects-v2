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
		virtual const bool& ReceiveEvents(const AL::Event& al_event) = 0;
	};
}


