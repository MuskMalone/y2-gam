#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       EventManager.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief      handles all the events

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Event.hpp"
#include "Types.hpp"
#include <functional>
#include <list>
#include <unordered_map>


class EventManager
{
public:/*  _________________________________________________________________________ */
/*! AddListener

@param eventId The ID of the event to listen for.
@param listener The function to be called when the event is triggered.

@return none.

Adds a listener function for a specific event ID. When an event with the 
specified ID is sent, the listener function will be called.
*/
	void AddListener(EventId eventId, std::function<void(Event&)> const& listener)
	{
		listeners[eventId].push_back(listener);
	}
	/*  _________________________________________________________________________ */
/*! SendEvent

@param event The event to be sent.

@return none.

Sends the specified event to all registered listeners for that event type.
Each listener function is called with the event as an argument.
*/

	void SendEvent(Event& event)
	{
		uint32_t type = event.GetType();

		for (auto const& listener : listeners[type])
		{
			listener(event);
		}
	}

	/*  _________________________________________________________________________ */
	/*! SendEvent

	@param eventId The ID of the event to be sent.

	@return none.

	Creates and sends an event with the specified ID to all registered listeners
	for that event type. Each listener function is called with the event as an argument.
	*/
	void SendEvent(EventId eventId)
	{
		Event event(eventId);

		for (auto const& listener : listeners[eventId])
		{
			listener(event);
		}
	}

private:
	std::unordered_map<EventId, std::list<std::function<void(Event&)>>> listeners;
};
