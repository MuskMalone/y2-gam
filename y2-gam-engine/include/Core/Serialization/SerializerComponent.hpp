//include this as a component to serialize all entities in the system
//remember to clear out this system everytime there is a change of state
#pragma once
#include <rapidjson/document.h>
namespace Serializer {
	struct SerializerComponent {
		void Serialize() {}; // intentionally left blank to pass the componentmgr check
	};
}
