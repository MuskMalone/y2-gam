#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       Physics.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		for physics

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Core/System.hpp"
/******************************************************************************/
/*!
\par        Image Engine
\file       PhysicsSystem.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		for physics resolve

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <Components/RigidBody.hpp>
#include "Math/MathUtils.h"
#include <Core/Physics.hpp>
#include <unordered_map>
#include <Core/Types.hpp>
#include <Core/Event.hpp>
namespace Physics {
	class PhysicsSystem : public System
	{
	public:

		void Init();

		void PreCollisionUpdate(float dt);
		void PostCollisionUpdate(float dt);

	private:
		const size_t iterations {10}; // iterations for sequential impulse
		ArbiterHashTable mArbiterTable;
		void CollisionListener(Event& event);
	};
	
}
