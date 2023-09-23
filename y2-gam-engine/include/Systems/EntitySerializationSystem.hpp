#pragma once

#include "Core/System.hpp"
#include <Core/Globals.hpp>
#include <utility>
#include <Core/Types.hpp>
class Event;

namespace Serializer {
	class EntitySerializationSystem : public System
	{
	public:

		void Init();
		void LoadEntities(std::string const& name);
		void FlushEntities(std::string const& name);
	private:
		void EntityEventListener(Event& event);
	};
}