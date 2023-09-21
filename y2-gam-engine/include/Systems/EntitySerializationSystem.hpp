#pragma once

#include "Core/System.hpp"
#include <Core/Globals.hpp>
#include <utility>
namespace Serializer {
	class EntitySerializationSystem : public System
	{
	public:
		void Init();
		void LoadEntities();
		void FlushEntities();
	};
}