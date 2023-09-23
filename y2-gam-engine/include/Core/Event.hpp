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
		if (mData.find(id) == mData.end()) {
			mGetFail = true;
			return T{};
		}
		mGetFail = false;
		return std::any_cast<T>(mData[id]);
	}

	EventId GetType() const
	{
		return mType;
	}

	bool GetFail() const {
		return mGetFail;
	}

private:
	bool mGetFail{false}; //failbit
	EventId mType{};
	std::unordered_map<EventId, std::any> mData{};
};
