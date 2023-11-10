/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptCoordinator.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 26, 2023

\brief      Communicates with the C# scripts, allowing for internal calls
						in which the information from CPP code can be accessed in C#,
						and vice versa.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
						or disclosure of this file or its contents without the prior
						written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"

#include "Scripting/ScriptCoordinator.hpp"
#include "Scripting/NodeManager.hpp"

#include "Core/Coordinator.hpp"

#include "Systems/InputSystem.hpp"
#include "Systems/CollisionSystem.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

namespace Image {

#define IMAGE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Image.InternalCalls::" #Name, Name)

	// For Engine Core
	/*  _________________________________________________________________________ */
	/*! EngineCore_IsEditorMode

	@param isEditorMode

	@return none.

	Get the editor mode flage of the engine in C#.
  */
	static void EngineCore_IsEditorMode(bool* isEditorMode) {
		::gCoordinator = Coordinator::GetInstance();
		*isEditorMode = gCoordinator->GetSystem<RenderSystem>()->IsEditorMode();
	}

	static void EngineCore_SetText(uint32_t entityID, MonoString** tag) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<Text>(entityID)) {
			gCoordinator->GetComponent<Text>(entityID).text = mono_string_to_utf8(*tag);
		}
	}


	// For Pathfinding
	/*  _________________________________________________________________________ */
	/*! PathfindingComponent_GetPath

	@param entityID
	The ID of the entity.

	@param closestNode
	The closest node to the entity.

	@param nextNode
	The next node to the entity.

	@param tag
	The tag of the entity.

	@return none.

	Calls the pathfinding algorithm in CPP, and returns the closest node and the
	next node to the entity in C#.
	*/
	static void PathfindingComponent_GetPath(uint32_t entityID, Vec2* closestNode, Vec2* nextNode, Vec2* tag) {
		if (!NodeManager::GetCurrentlyActiveNodes().empty()) {
			::gCoordinator = Coordinator::GetInstance();
			NodeManager::Path path{ NodeManager::DjkstraAlgorithm(NodeManager::FindClosestNodeToEntity(entityID),
				NodeManager::FindClosestNodeToEntity(::gCoordinator->GetSystem<RenderSystem>()->mPlayer)) };

			*closestNode = { gCoordinator->GetComponent<Transform>(path.front()).position.x,
				gCoordinator->GetComponent<Transform>(path.front()).position.y };

			if (path.size() > 1) {
				*nextNode = gCoordinator->GetComponent<Transform>(path[1]).position.x,
					gCoordinator->GetComponent<Transform>(path[1]).position.y;
				*tag = { static_cast<float>(gCoordinator->GetComponent<Node>(path[0]).type), 
					static_cast<float>(gCoordinator->GetComponent<Node>(path[1]).type) };
			}
			else {
				*nextNode = { gCoordinator->GetComponent<Transform>(path.front()).position.x,
				gCoordinator->GetComponent<Transform>(path.front()).position.y };
				*tag = { static_cast<float>(gCoordinator->GetComponent<Node>(path[0]).type), 
					static_cast<float>(gCoordinator->GetComponent<Node>(path[0]).type) };
			}
    }
	}
	
	// For Physics
	/*  _________________________________________________________________________ */
	/*! PhysicsComponent_GetRaycast

	@param origin
	The origin of the raycast.

	@param end
	The end of the raycast.

	@param raycastHit
	The raycast hit information.

	@return none.

	Get the raycast hit information in C#. Wraps the raycast function in CPP for
	calling in C#.
	*/
	static void PhysicsComponent_GetRaycast(Vec2 origin, Vec2 end, uint32_t* entityToIgnore, bool* hit, Vec2* normal,
		Vec2* point, float* distance, uint32_t* entityID, MonoString** tag) {
		::gCoordinator = Coordinator::GetInstance();
		Physics::RayHit rh{};
		*hit = ::gCoordinator->GetSystem<Collision::CollisionSystem>()->Raycast(origin, end, rh, *entityToIgnore);
		
		*normal = rh.normal;
		*point = rh.point;
		*distance = rh.distance;
		*entityID = rh.entityID;
		
		if (gCoordinator->HasComponent<Tag>(rh.entityID)) {
			*tag = mono_string_new(mono_domain_get(), gCoordinator->GetComponent<Tag>(rh.entityID).tag.c_str());
		}
		else {
			*tag = mono_string_new(mono_domain_get(), "No Tag");
		}
	}

	// For Graphics
	/*  _________________________________________________________________________ */
	/*! AnimationComponent_GetAnimationState

	@param entityID
	The ID of the entity.

	@param outAnimationState
	The current animation state of the entity.

	@return none.

	Get the current animation state of the entity in C#.
	*/
	static void AnimationComponent_GetAnimationState(uint32_t entityID, int* outAnimationState) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<Animation>(entityID))
			*outAnimationState = static_cast<int>(gCoordinator->GetComponent<Animation>(entityID).currState);
	}

	/*  _________________________________________________________________________ */
	/*! AnimationComponent_SetAnimationState

	@param entityID
	The ID of the entity.

	@param animationState
	Updated animation state of the entity.

	@return none.

	Set the current animation state of the entity in C#.
	*/
	static void AnimationComponent_SetAnimationState(uint32_t entityID, int* animationState) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<Animation>(entityID))
			gCoordinator->GetComponent<Animation>(entityID).currState = static_cast<ANIM_STATE>(*animationState);
	}

	/*  _________________________________________________________________________ */
/*! GraphicsComponent_GetScale

@param entityID
The ID of the entity.

@param outScale
The current scale of the entity.

@return none.

Get the current scale of the entity in C#.
*/
	static void GraphicsComponent_GetScale(uint32_t entityID, Vec3* outScale) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<Transform>(entityID)) {
			*outScale = Vec3{ gCoordinator->GetComponent<Transform>(entityID).scale.x,
			gCoordinator->GetComponent<Transform>(entityID).scale.y,
			gCoordinator->GetComponent<Transform>(entityID).scale.z };
		}
	}

	/*  _________________________________________________________________________ */
	/*! GraphicsComponent_SetScale

	@param entityID
	The ID of the entity.

	@param scale
	Updated scale of the entity.

	@return none.

	Set the current scale of the entity in C#.
	*/
	static void GraphicsComponent_SetScale(uint32_t entityID, Vec3* scale) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<Transform>(entityID)) {
			gCoordinator->GetComponent<Transform>(entityID).scale =
			{ scale->x, scale->y, scale->z };
		}
	}

	// For Translation
	/*  _________________________________________________________________________ */
	/*! TransformComponent_GetTranslation

	@param entityID
	The ID of the entity.

	@param outTranslation
	The current position of the entity.

	@return none.

	Get the current position of the entity in C#.
	*/
	static void TransformComponent_GetTranslation(uint32_t entityID, Vec2* outTranslation) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<Transform>(entityID)) {
			*outTranslation = Vec2{ gCoordinator->GetComponent<Transform>(entityID).position.x,
				gCoordinator->GetComponent<Transform>(entityID).position.y };
		}
	}

	/*  _________________________________________________________________________ */
	/*! TransformComponent_SetTranslation

	@param entityID
	The ID of the entity.

	@param translation
	Updated position of the entity.

	@return none.

	Set the current position of the entity in C#.
	*/
	static void TransformComponent_SetTranslation(uint32_t entityID, Vec2* translation) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<Transform>(entityID)) {
			gCoordinator->GetComponent<Transform>(entityID).position = { translation->x,
				translation->y,
				gCoordinator->GetComponent<Transform>(entityID).position.z };
		}
	}

	// For Force
	/*  _________________________________________________________________________ */
	/*! ForceComponent_GetForce

	@param entityID
	The ID of the entity.

	@param outForce
	The current force.

	@return none.

	Get the current force of the entity in C#.
	*/
	static void ForceComponent_GetForce(uint32_t entityID, Vec2* outForce) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<RigidBody>(entityID)) {
			*outForce = gCoordinator->GetComponent<RigidBody>(entityID).force;
		}
	}

	/*  _________________________________________________________________________ */
	/*! ForceComponent_SetForce

	@param entityID
	The ID of the entity.

	@param force
	The updated force.

	@return none.

	Set the current force of the entity in C#.
	*/
	static void ForceComponent_SetForce(uint32_t entityID, Vec2* force) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<RigidBody>(entityID)) {
			gCoordinator->GetComponent<RigidBody>(entityID).force = *force;
		}
	}

	/*  _________________________________________________________________________ */
	/*! ForceComponent_GetMass

	@param entityID
	The ID of the entity.

	@param outMass
	The current mass.

	@return none.

	Get the current mass of the entity in C#.
	*/
	static void ForceComponent_GetMass(uint32_t entityID, float* outMass) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<RigidBody>(entityID)) {
			*outMass = gCoordinator->GetComponent<RigidBody>(entityID).mass;
		}
	}

	/*  _________________________________________________________________________ */
	/*! ForceComponent_SetMass

	@param entityID
	The ID of the entity.

	@param mass
	The updated mass.

	@return none.

	Set the current mass of the entity in C#.
	*/
	static void ForceComponent_SetMass(uint32_t entityID, float* mass) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<RigidBody>(entityID)) {
			gCoordinator->GetComponent<RigidBody>(entityID).mass = *mass;
		}
	}

	/*  _________________________________________________________________________ */
	/*! ForceComponent_GetVelocity

	@param entityID
	The ID of the entity.

	@param outVelocity
	The current velocity

	@return none.

	Get the current velocity of the entity in C#.
	*/
	static void ForceComponent_GetVelocity(uint32_t entityID, Vec2* outVelocity) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<RigidBody>(entityID)) {
			*outVelocity = gCoordinator->GetComponent<RigidBody>(entityID).velocity;
		}
	}

	/*  _________________________________________________________________________ */
	/*! ForceComponent_SetVelocity

	@param entityID
	The ID of the entity.

	@param velocity
	The updated velocity

	@return none.

	Set the current velocity of the entity in C#.
	*/
	static void ForceComponent_SetVelocity(uint32_t entityID, Vec2* velocity) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<RigidBody>(entityID)) {
			gCoordinator->GetComponent<RigidBody>(entityID).velocity = *velocity;
		}
	}

	// For Input
	/*  _________________________________________________________________________ */
	/*! Input_IsKeyPressed

	@param key
	The current key to check.

	@return bool
	Returns true if pressed.

	Check if the key is being pressed.
	*/
	static bool Input_IsKeyPressed(size_t key) {
		::gCoordinator = Coordinator::GetInstance();
		auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
		return inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, key);
	}

	/*  _________________________________________________________________________ */
	/*! Input_IsKeyClicked

	@param key
	The current key to check.

	@return bool
	Returns true if clicked.

	Check if the key is being clicked.
	*/
	static bool Input_IsKeyClicked(size_t key) {
		::gCoordinator = Coordinator::GetInstance();
		auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
		return inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, key);
	}

	/*  _________________________________________________________________________ */
	/*! Input_IsKeyReleased

	@param key
	The current key to check.

	@return bool
	Returns true if released.

	Check if the key is being released.
	*/
	static bool Input_IsKeyReleased(size_t key) {
		::gCoordinator = Coordinator::GetInstance();
		auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
		return inputSystem->CheckKey(InputSystem::InputKeyState::KEY_RELEASED, key);
	}

	/*  _________________________________________________________________________ */
	/*! Input_IsMousePressed

	@param key
	The current key to check.

	@return bool
	Returns true if pressed.

	Check if the mouse key is being pressed.
	*/
	static bool Input_IsMousePressed(size_t key) {
		::gCoordinator = Coordinator::GetInstance();
		auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
		return inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_PRESSED, key);
	}

	/*  _________________________________________________________________________ */
	/*! Input_IsMouseClicked

	@param key
	The current key to check.

	@return bool
	Returns true if clicked.

	Check if the mouse key is being clicked.
	*/
	static bool Input_IsMouseClicked(size_t key) {
		::gCoordinator = Coordinator::GetInstance();
		auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
		return inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, key);
	}

	/*  _________________________________________________________________________ */
	/*! Input_IsMouseReleased

	@param key
	The current key to check.

	@return bool
	Returns true if released

	Check if the mouse key is being released.
	*/
	static bool Input_IsMouseReleased(size_t key) {
		::gCoordinator = Coordinator::GetInstance();
		auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
		return inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_RELEASED, key);
	}

	/*  _________________________________________________________________________ */
	/*! RegisterFunctions

	@return none.

	Calls mono_add_internal_call on the respective functions, so that the C# code 
	can access it.
	*/
	void ScriptCoordinator::RegisterFunctions() {
		IMAGE_ADD_INTERNAL_CALL(EngineCore_IsEditorMode);
		IMAGE_ADD_INTERNAL_CALL(EngineCore_SetText);

		IMAGE_ADD_INTERNAL_CALL(PathfindingComponent_GetPath);
		IMAGE_ADD_INTERNAL_CALL(PhysicsComponent_GetRaycast);

		IMAGE_ADD_INTERNAL_CALL(AnimationComponent_GetAnimationState);
		IMAGE_ADD_INTERNAL_CALL(AnimationComponent_SetAnimationState);
		IMAGE_ADD_INTERNAL_CALL(GraphicsComponent_GetScale);
		IMAGE_ADD_INTERNAL_CALL(GraphicsComponent_SetScale);

		IMAGE_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		IMAGE_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		IMAGE_ADD_INTERNAL_CALL(ForceComponent_GetForce);
		IMAGE_ADD_INTERNAL_CALL(ForceComponent_SetForce);
		IMAGE_ADD_INTERNAL_CALL(ForceComponent_GetMass);
		IMAGE_ADD_INTERNAL_CALL(ForceComponent_SetMass);
		IMAGE_ADD_INTERNAL_CALL(ForceComponent_GetVelocity);
		IMAGE_ADD_INTERNAL_CALL(ForceComponent_SetVelocity);

		IMAGE_ADD_INTERNAL_CALL(Input_IsKeyPressed);
		IMAGE_ADD_INTERNAL_CALL(Input_IsKeyClicked);
		IMAGE_ADD_INTERNAL_CALL(Input_IsKeyReleased);
		IMAGE_ADD_INTERNAL_CALL(Input_IsMousePressed);
		IMAGE_ADD_INTERNAL_CALL(Input_IsMouseClicked);
		IMAGE_ADD_INTERNAL_CALL(Input_IsMouseReleased);
	}
}
