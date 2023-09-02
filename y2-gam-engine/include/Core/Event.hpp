#pragma once

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

	template<typename T>
	void SetParam(EventId id, T value)
	{
		mData[id] = value;
	}

	template<typename T>
	T GetParam(EventId id)
	{
		assert(std::is_default_constructible<T>::value);
		if (mData.find(id) == mData.end()) return T{};
		return std::any_cast<T>(mData[id]);
	}

	EventId GetType() const
	{
		return mType;
	}

private:
	EventId mType{};
	std::unordered_map<EventId, std::any> mData{};
};
