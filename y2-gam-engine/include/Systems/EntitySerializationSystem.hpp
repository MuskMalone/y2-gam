/******************************************************************************/
/*!
\par        Entity serialization System
\file       EntitySerializationSystem.hpp

\author     Ng Wen Wen (wenwen.ng@digipen.edu)
\date       Sep 10, 2023

\brief      Implementation file for entity serialization

			This file contains the Entity Serialization System that facilitates
			the creation, manipulation, and file storage of JSON data for entities
			in a software application or game.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
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