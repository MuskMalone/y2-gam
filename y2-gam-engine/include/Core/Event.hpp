#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       Event.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief      Event data type that is sent

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Types.hpp"
#include <any>
#include <unordered_map>
#include <type_traits>


class Event
{
public:
	Event() = delete;

	explicit Event(EventId type)
		: mType(type)
	{}
	/*  _________________________________________________________________________ */
/*! SetParam

@param id The ID of the parameter to be set.
@param value The value to be set for the parameter.

@return none.

Sets a parameter with the specified ID and value for the event.
*/
	template<typename T>
	void SetParam(EventId id, T value)
	{
		mData[id] = value;
	}
	/*  _________________________________________________________________________ */
/*! GetParam

@param id The ID of the parameter to be retrieved.

@return T The value of the parameter with the specified ID.

Retrieves the value of a parameter with the specified ID. If the parameter
does not exist, returns a default-constructed value of type T and sets the
failure flag.
*/

	template<typename T>
	T GetParam(EventId id)
	{
		static_assert(std::is_default_constructible<T>::value);
		if (mData.find(id) == mData.end()) {
			mGetFail = true;
			return T{};
		}
		mGetFail = false;
		return std::any_cast<T>(mData[id]);
	}
	/*  _________________________________________________________________________ */
/*! GetType

@return EventId The type ID of the event.

Provides the type ID of the event.
*/
	EventId GetType() const
	{
		return mType;
	}
	/*  _________________________________________________________________________ */
/*! GetFail

@return bool Whether the last GetParam call failed.

Provides the status of the last GetParam call. Returns true if the call failed,
otherwise false.
*/
	bool GetFail() const {
		return mGetFail;
	}

private:
	bool mGetFail{false}; //failbit
	EventId mType{};
	std::unordered_map<EventId, std::any> mData{};
};
