/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptCoordinator.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 25, 2023

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
#include "Scripting/ScriptManager.hpp"

#include "Core/Coordinator.hpp"
#include "Core/FrameRateController.hpp"

#include "Graphics/SpriteManager.hpp"

#include "Systems/InputSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/CollisionSystem.hpp"

#include "Engine/PrefabsManager.hpp"
#include "Engine/SceneManager.hpp"

#include "Audio/Sound.hpp"

using namespace Physics;

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

namespace Image {

#define IMAGE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Image.InternalCalls::" #Name, Name)
	// For Gameplay
	/*  _________________________________________________________________________ */
	/*! GameplayComponent_FireCard

	@param startPos
	The starting position the card will be fired at.

	@return none.

	Fires an entity with the Card.cs script attached.
	*/
	static void GameplayComponent_FireCard(Vec2* startPos) {
		/*
		const float cardSize{ 5.f };
		Entity newEntity = gCoordinator->CreateEntity();
		gCoordinator->AddComponent(
			newEntity,
			Layering{ "Card" });

		gCoordinator->AddComponent(
			newEntity,
			Transform{
					{startPos->x,startPos->y,0},
					{0,0,0},
					{cardSize,cardSize,cardSize}
			});

		gCoordinator->AddComponent(
			newEntity,
			Gravity{
				Vec2{0.f, 0.f}
			});

		gCoordinator->AddComponent(
			newEntity,
			RigidBody{
					Vec2{startPos->x,startPos->y},
					0.f,
					0.f,
					Vec2{cardSize,cardSize}
			});

		gCoordinator->AddComponent(
			newEntity,
			Collider{ Vec2{startPos->x,startPos->y},0,Vec2{} });

		gCoordinator->AddComponent(
			newEntity,
			Tag{ "Card" });
		//gCoordinator->AddComponent(

			//);
		gCoordinator->AddComponent(
			newEntity,
			Script{ "ObjectCard" }
			);
		*/
		//PrefabsManager::GetInstance()->SpawnPrefab("Card", *startPos);
	}

	/*  _________________________________________________________________________ */
	/*! GameplayComponent_SpawnPrefab

	@param entityID
	The entity to destroy.

	@return none.

	Destroys entity.
	*/
	static void GameplayComponent_SpawnPrefab(MonoString** fileName, Vec2* startPos) {
		const char* utf8Str = *fileName != nullptr ? mono_string_to_utf8(*fileName) : nullptr;
		if (utf8Str != nullptr) {
			PrefabsManager::GetInstance()->SpawnPrefab(utf8Str, *startPos);
		}
#ifndef _INSTALLER
		else {
			LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Invalid String Parameter!"
				, __FUNCTION__);
		}
#endif
	}

	/*  _________________________________________________________________________ */
	/*! GameplayComponent_Destroy

	@param entityID
	The entity to destroy.

	@return none.

	Destroys entity.
	*/
	static void GameplayComponent_Destroy(uint32_t* entityID) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			gCoordinator->DestroyEntity(*entityID);
			Image::ScriptManager::RemoveEntity(*entityID);
		}
	}

	/*  _________________________________________________________________________ */
	/*! GameplayComponent_GetPlayerPos

	@param playerPos
	The position of the player.

	@return none.

	Get the player position.
	*/
	static void GameplayComponent_GetPlayerPos(Vec2* playerPos) {
		::gCoordinator = Coordinator::GetInstance();
		Entity player{};
		for (auto const& e : gCoordinator->GetSystem<RenderSystem>()->mEntities) {
			if (!::gCoordinator->HasComponent<Tag>(e)) continue;
			auto const& tag = ::gCoordinator->GetComponent<Tag>(e);
			if (tag.tag == "Player") {
				player = e;
				break;
			}
		}

		*playerPos = gCoordinator->GetComponent<Collider>(player).position;
	}

	/*  _________________________________________________________________________ */
	/*! GameplayComponent_GetPlayerID

	@param playerID
	Id of the player.

	@return none.

	Get the player id.
	*/
		static void GameplayComponent_GetPlayerID(uint32_t* playerID) {
			if (*playerID >= 0 && *playerID < MAX_ENTITIES) {
				::gCoordinator = Coordinator::GetInstance();
				Entity player{};
				for (auto const& e : gCoordinator->GetSystem<RenderSystem>()->mEntities) {
					if (!::gCoordinator->HasComponent<Tag>(e)) continue;
					auto const& tag = ::gCoordinator->GetComponent<Tag>(e);
					if (tag.tag == "Player") {
						player = e;
						break;
					}
				}

				*playerID = player;
			}
		}

		/*  _________________________________________________________________________ */
		/*! GameplayComponent_GetEntityIDByTag

		@param tag
		The tag you want to search for.

		@return none.

		Get the ID of an entity by its tag.
		*/
		static void GameplayComponent_GetEntityIDByTag(uint32_t* entityID, MonoString** tag) {
			if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
				const char* utf8Str = *tag != nullptr ? mono_string_to_utf8(*tag) : nullptr;
				if (utf8Str != nullptr) {
					::gCoordinator = Coordinator::GetInstance();
					for (auto& ent : gCoordinator->GetSystem<RenderSystem>()->mEntities) {
						if (gCoordinator->HasComponent<Tag>(ent)) {
							if (gCoordinator->GetComponent<Tag>(ent).tag == utf8Str) {
								*entityID = ent;
								break;
							}
						}
					}
				}
#ifndef _INSTALLER
				else {
					LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Invalid String Parameter!"
						, __FUNCTION__);
				}
#endif
			}
		}

	/*  _________________________________________________________________________ */
	/*! GameplayComponent_IsSwappable

	@param entityID
	The ID of the entity.

	@param bool
	The swappable flag.

	@return none.

	Checks if the entity is swappable or not.
	*/
	static void GameplayComponent_IsSwappable(uint32_t* entityID, bool* outIsSwappable) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Swappable>(*entityID)) {
				*outIsSwappable = true;
			}
			else {
				*outIsSwappable = false;
			}
		}
	}

	/*  _________________________________________________________________________ */
	/*! GameplayComponent_Swap

	@param lhs
	The lhs entity.

	@param rhs
	The rhs entity.

	@return none.

	Swaps the positions of two different entities.
	*/
	static void GameplayComponent_Swap(uint32_t* lhs, uint32_t* rhs) {
		::gCoordinator = Coordinator::GetInstance();
		if (gCoordinator->HasComponent<Transform>(*lhs) && gCoordinator->HasComponent<Transform>(*rhs) &&
			gCoordinator->HasComponent<Collider>(*lhs) && gCoordinator->HasComponent<Collider>(*rhs)) {
			auto& lhsTransform{ gCoordinator->GetComponent<Transform>(*lhs).position };
			auto& rhsTransform{ gCoordinator->GetComponent<Transform>(*rhs).position };
			auto& lhsCollider{ gCoordinator->GetComponent<Collider>(*lhs).position };
			auto& rhsCollider{ gCoordinator->GetComponent<Collider>(*rhs).position };

			glm::vec2 lhsOffset{ glm::vec2(lhsTransform.x - lhsCollider.x, lhsTransform.y - lhsCollider.y) };
			glm::vec2 rhsOffset{ glm::vec2(rhsTransform.x - rhsCollider.x, rhsTransform.y - rhsCollider.y) };

			std::swap(lhsTransform, rhsTransform);
			std::swap(lhsCollider, rhsCollider);

			lhsCollider = Vec2(lhsCollider.x - lhsOffset.x, lhsCollider.y - lhsOffset.y);
			rhsCollider = Vec2(rhsCollider.x - rhsOffset.x, rhsCollider.y - rhsOffset.y);
		}
	}

	/*  _________________________________________________________________________ */
	/*! GameplayComponent_SlowdownTime

	@param flag
	Set slowdown time to true or false.

	@return none.

	Set slowdown time to true or false.
	*/
	static void GameplayComponent_SlowdownTime(bool* flag) {
		auto frameController{ FrameRateController::GetInstance() };
		if (*flag) {
			frameController->ScaleDeltaTime(0.4f);
		}
		else {
			frameController->ScaleDeltaTime(0.f);
		}
	}

	// For UI
	/*  _________________________________________________________________________ */
	/*! UIComponent_GetIsUIButtonClicked

	@param entityID
	The ID of the entity.

	@param outIsClicked
	The clicked flag.

	@return none.

	Gets if the UI button is clicked or not.
	*/
	static void UIComponent_GetIsUIButtonClicked(uint32_t* entityID, bool* outIsClicked) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<UIImage>(*entityID))
				*outIsClicked = gCoordinator->GetComponent<UIImage>(*entityID).isClicked;
		}
	}

	/*  _________________________________________________________________________ */
	/*! UIComponent_GetIsUIButtonHover

	@param entityID
	The ID of the entity.

	@param outIsHover
	The hover flag.

	@return none.

	Gets if the UI button is hovered over or not.
	*/
	static void UIComponent_GetIsUIButtonHover(uint32_t* entityID, bool* outIsHover) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<UIImage>(*entityID))
				*outIsHover = gCoordinator->GetComponent<UIImage>(*entityID).isHover;
		}
	}

	// For Serialization
	/*  _________________________________________________________________________ */
	/*! SerializationComponent_GetIsFacingRight

	@param entityID
	The ID of the entity.

	@param outFacingDirection
	Flag for if the entity is facing right or not.

	@return none.

	Gets the facing right flag in C#.
	*/
	static void SerializationComponent_GetIsFacingRight(uint32_t* entityID, bool* outFacingDirection) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Script>(*entityID))
				*outFacingDirection = gCoordinator->GetComponent<Script>(*entityID).isFacingRight;
		}
	}

	/*  _________________________________________________________________________ */
	/*! SerializationComponent_SetIsFacingRight

	@param entityID
	The ID of the entity.

	@param facingDirection
	Flag for if the entity is facing right or not.

	@return none.

	Sets the facing right flag in C#.
	*/
	static void SerializationComponent_SetIsFacingRight(uint32_t entityID, bool* facingDirection) {
		if (entityID >= 0 && entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Script>(entityID))
				gCoordinator->GetComponent<Script>(entityID).isFacingRight = *facingDirection;
		}
	}

	// For Engine Core
	/*  _________________________________________________________________________ */
	/*! EngineCore_Quit

	@return none.

	Quits the application.
	*/
	static void EngineCore_Quit() {
		Event e{ Events::Window::QUIT };
		gCoordinator->SendEvent(e);
	}

	/*  _________________________________________________________________________ */
	/*! EngineCore_GetMousePos

	@param outMousePos
	The mouse position.

	@return none.

	Gets the mouse pos in C#.
	*/
	static void EngineCore_GetMousePos(Vec2* outMousePos) {
		::gCoordinator = Coordinator::GetInstance();
		auto inputSystem{ ::gCoordinator->GetSystem<InputSystem>() };
		Vec2 mousePos{ inputSystem->GetSceneMousePos().first, inputSystem->GetSceneMousePos().second };
		*outMousePos = mousePos;
	}

	/*  _________________________________________________________________________ */
	/*! EngineCore_PlayAudio

	@param audioFileName

	@return none.

	Plays audio.
	*/
	static void EngineCore_PlayAudio(MonoString** audioFileName, int* loopCount) {
		const char* utf8Str = *audioFileName != nullptr ? mono_string_to_utf8(*audioFileName) : nullptr;
		if (utf8Str != nullptr) {
			SoundManager::AudioPlay(mono_string_to_utf8(*audioFileName), *loopCount);
			mono_free(*audioFileName);
		}

#ifndef _INSTALLER
		else {
			LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Invalid String Parameter!"
				, __FUNCTION__);
		}
#endif
	}

	/*  _________________________________________________________________________ */
	/*! EngineCore_LoadScene

	@param sceneName
	The scene name.

	@return none.

	Loads the specified scene.
	*/
	static void EngineCore_LoadScene(MonoString** sceneName) {
		const char* utf8Str = *sceneName != nullptr ? mono_string_to_utf8(*sceneName) : nullptr;
		if (utf8Str != nullptr) {
			SceneManager::GetInstance()->LoadScene(utf8Str);
		}

#ifndef _INSTALLER
		else {
			LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Invalid String Parameter!"
				, __FUNCTION__);
		}
#endif
	}

	/*  _________________________________________________________________________ */
	/*! EngineCore_IsEditorMode

	@param isEditorMode
	The flag for editor mode.

	@return none.

	Get the editor mode flag of the engine in C#.
  */
	static void EngineCore_IsEditorMode(bool* isEditorMode) {
		::gCoordinator = Coordinator::GetInstance();
		*isEditorMode = gCoordinator->GetSystem<RenderSystem>()->IsEditorMode();
	}

	/*  _________________________________________________________________________ */
	/*! EngineCore_SetText

	@param entityID
	The ID of the entity.

	@param tag
	The text to change the entity's text component to.

	@return none.

	Sets text for the entity.
	*/
	static void EngineCore_SetText(uint32_t entityID, MonoString** tag) {
		if (entityID >= 0 && entityID < MAX_ENTITIES) {
			const char* utf8Str = *tag != nullptr ? mono_string_to_utf8(*tag) : nullptr;
			if (utf8Str != nullptr) {
				::gCoordinator = Coordinator::GetInstance();
				if (gCoordinator->HasComponent<Text>(entityID)) {
					gCoordinator->GetComponent<Text>(entityID).text = mono_string_to_utf8(*tag);
				}
			}
#ifndef _INSTALLER
			else {
				LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Invalid String Parameter!"
					, __FUNCTION__);
			}
#endif
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
	static void PathfindingComponent_GetPath(uint32_t* entityID, Vec2* closestNode, Vec2* nextNode, Vec2* tag) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			if (!NodeManager::GetCurrentlyActiveNodes().empty()) {
				::gCoordinator = Coordinator::GetInstance();
				NodeManager::Path path{ NodeManager::DjkstraAlgorithm(NodeManager::FindClosestNodeToEntity(*entityID),
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
	}
	
	// For Physics
	/*  _________________________________________________________________________ */
	/*! PhysicsComponent_Collided

	@param 

	@return none.

	Get the raycast hit information in C#. Wraps the raycast function in CPP for
	calling in C#.
	*/
	static void PhysicsComponent_Collided(uint32_t* entityID, bool* collidedOrNot) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			bool collided{ gCoordinator->GetSystem<Collision::CollisionSystem>()->IsIntersected(*entityID).size() > 0 };
			*collidedOrNot = collided;
		}
	}

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
	static void PhysicsComponent_GetRaycast(Vec2* origin, Vec2* end, uint32_t entityToIgnore, bool* hit, uint32_t* entityHandle,
		MonoString** tag, MonoString** layer) {
		::gCoordinator = Coordinator::GetInstance();
		Physics::RayHit rh{};
		*hit = ::gCoordinator->GetSystem<Collision::CollisionSystem>()->Raycast(*origin, *end, rh, entityToIgnore);
		if (rh.entityID >= 0 && rh.entityID < MAX_ENTITIES) {
			*entityHandle = rh.entityID;

			if (gCoordinator->HasComponent<Tag>(rh.entityID)) {
				if (gCoordinator->GetComponent<Tag>(rh.entityID).tag.c_str() != nullptr)
					*tag = mono_string_new(mono_domain_get(), gCoordinator->GetComponent<Tag>(rh.entityID).tag.c_str());
			}

			if (gCoordinator->HasComponent<Layering>(rh.entityID)) {
				if (gCoordinator->GetComponent<Layering>(rh.entityID).assignedLayer.c_str() != nullptr)
					*layer = mono_string_new(mono_domain_get(), gCoordinator->GetComponent<Layering>(rh.entityID).assignedLayer.c_str());
			}
		}
	}

	/*  _________________________________________________________________________ */
	/*! PhysicsComponent_GetColliderDimensions

	@param entityID
	The ID of the entity.

	@param outDim
	The current collider dimensions of the entity.

	@return none.

	Get the collider dimensions of the entity in C#.
	*/
	static void PhysicsComponent_GetColliderDimensions(uint32_t* entityID, Vec2* outDim) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Collider>(*entityID)) {
				*outDim = Vec2{ gCoordinator->GetComponent<Collider>(*entityID).dimension };
			}
		}
	}

	/*  _________________________________________________________________________ */
	/*! PhysicsComponent_GetColliderPos

	@param entityID
	The ID of the entity.

	@param outPos
	The current collider position of the entity.

	@return none.

	Get the collider current position of the entity in C#.
	*/
	static void PhysicsComponent_GetColliderPos(uint32_t* entityID, Vec2* outPos) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Collider>(*entityID)) {
				*outPos = Vec2{ gCoordinator->GetComponent<Collider>(*entityID).position };
			}
		}
	}

	/*  _________________________________________________________________________ */
	/*! PhysicsComponent_SetColliderPos

	@param entityID
	The ID of the entity.

	@param translation
	Updated  collider position of the entity.

	@return none.

	Set the collider current position of the entity in C#.
	*/
	static void PhysicsComponent_SetColliderPos(uint32_t entityID, Vec2* pos) {
		if (entityID >= 0 && entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Collider>(entityID)) {
				gCoordinator->GetComponent<Collider>(entityID).position = *pos;
			}
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
	static void AnimationComponent_GetAnimationState(uint32_t* entityID, int* outAnimationState) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Animation>(*entityID))
				*outAnimationState = static_cast<int>(gCoordinator->GetComponent<Animation>(*entityID).currState);
		}
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
		if (entityID >= 0 && entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Animation>(entityID))
				gCoordinator->GetComponent<Animation>(entityID).currState = static_cast<uint64_t>(*animationState);
		}
	}

	/*  _________________________________________________________________________ */
	/*! GraphicsComponent_SetSprite

	@param entityID
	The ID of the entity.

	@param fileName
	The name of the file the sprite asset is at.

	@return none.

	Sets the sprite for the entity.
	*/
	static void GraphicsComponent_SetSprite(uint32_t entityID, MonoString** fileName) {
		if (entityID >= 0 && entityID < MAX_ENTITIES) {
			const char* utf8Str = *fileName != nullptr ? mono_string_to_utf8(*fileName) : nullptr;
			if (utf8Str != nullptr) {
				::gCoordinator = Coordinator::GetInstance();
				if (gCoordinator->HasComponent<Sprite>(entityID)) {
					gCoordinator->GetComponent<Sprite>(entityID).spriteID =
						SpriteManager::GetResourceID(mono_string_to_utf8(*fileName));
				}
			}
#ifndef _INSTALLER
			else {
				LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Invalid String Parameter!"
					, __FUNCTION__);
			}
#endif
		}
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
	static void GraphicsComponent_GetScale(uint32_t* entityID, Vec3* outScale) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Transform>(*entityID)) {
				*outScale = Vec3{ gCoordinator->GetComponent<Transform>(*entityID).scale.x,
				gCoordinator->GetComponent<Transform>(*entityID).scale.y,
				gCoordinator->GetComponent<Transform>(*entityID).scale.z };
			}
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
		if (entityID >= 0 && entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Transform>(entityID)) {
				gCoordinator->GetComponent<Transform>(entityID).scale =
				{ scale->x, scale->y, scale->z };
			}
		}
	}

	/*  _________________________________________________________________________ */
	/*! GraphicsComponent_GetRotation

	@param entityID
	The ID of the entity.

	@param outRotation
	The current rotation of the entity.

	@return none.

	Get the current rotation of the entity in C#.
	*/
	static void GraphicsComponent_GetRotation(uint32_t* entityID, Vec3* outRotation) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Transform>(*entityID)) {
				*outRotation = Vec3{ gCoordinator->GetComponent<Transform>(*entityID).rotation.x,
				gCoordinator->GetComponent<Transform>(*entityID).rotation.y,
				gCoordinator->GetComponent<Transform>(*entityID).rotation.z };
			}
		}
	}

	/*  _________________________________________________________________________ */
	/*! GraphicsComponent_SetRotation

	@param entityID
	The ID of the entity.

	@param rotation
	Updated rotation of the entity.

	@return none.

	Set the current rotation of the entity in C#.
	*/
	static void GraphicsComponent_SetRotation(uint32_t entityID, Vec3* rotation) {
		if (entityID >= 0 && entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Transform>(entityID)) {
				gCoordinator->GetComponent<Transform>(entityID).rotation =
				{ rotation->x, rotation->y, rotation->z };
			}
		}
	}

	/*  _________________________________________________________________________ */
	/*! GraphicsComponent_SetColour

	@param entityID
	The ID of the entity.

	@param colour
	The colour to set to.

	@return none.

	Set the current colour of the entity sprite in C#.
	*/
	static void GraphicsComponent_SetColour(uint32_t entityID, glm::vec4* colour) {
		if (entityID >= 0 && entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();

			if (gCoordinator->HasComponent<Sprite>(entityID)) {
				gCoordinator->GetComponent<Sprite>(entityID).color = *colour;
			}
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
	static void TransformComponent_GetTranslation(uint32_t* entityID, Vec2* outTranslation) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Transform>(*entityID)) {
				*outTranslation = Vec2{ gCoordinator->GetComponent<Transform>(*entityID).position.x,
					gCoordinator->GetComponent<Transform>(*entityID).position.y };
			}
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
		if (entityID >= 0 && entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<Transform>(entityID)) {
				gCoordinator->GetComponent<Transform>(entityID).position = { translation->x,
					translation->y,
					gCoordinator->GetComponent<Transform>(entityID).position.z };
			}

			if (gCoordinator->HasComponent<Collider>(entityID)) {
				gCoordinator->GetComponent<Collider>(entityID).position = { translation->x,
					translation->y };
			}
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
	static void ForceComponent_GetForce(uint32_t* entityID, Vec2* outForce) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<RigidBody>(*entityID)) {
				*outForce = gCoordinator->GetComponent<RigidBody>(*entityID).force;
			}
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
		if (entityID >= 0 && entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<RigidBody>(entityID)) {
				gCoordinator->GetComponent<RigidBody>(entityID).force = *force;
			}
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
	static void ForceComponent_GetMass(uint32_t* entityID, float* outMass) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<RigidBody>(*entityID)) {
				*outMass = gCoordinator->GetComponent<RigidBody>(*entityID).mass;
			}
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
		if (entityID >= 0 && entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<RigidBody>(entityID)) {
				gCoordinator->GetComponent<RigidBody>(entityID).mass = *mass;
			}
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
	static void ForceComponent_GetVelocity(uint32_t* entityID, Vec2* outVelocity) {
		if (*entityID >= 0 && *entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<RigidBody>(*entityID)) {
				*outVelocity = gCoordinator->GetComponent<RigidBody>(*entityID).velocity;
			}
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
		if (entityID >= 0 && entityID < MAX_ENTITIES) {
			::gCoordinator = Coordinator::GetInstance();
			if (gCoordinator->HasComponent<RigidBody>(entityID)) {
				gCoordinator->GetComponent<RigidBody>(entityID).velocity = *velocity;
			}
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
		IMAGE_ADD_INTERNAL_CALL(GameplayComponent_FireCard);
		IMAGE_ADD_INTERNAL_CALL(GameplayComponent_SpawnPrefab);
		IMAGE_ADD_INTERNAL_CALL(GameplayComponent_Destroy);
		IMAGE_ADD_INTERNAL_CALL(GameplayComponent_GetPlayerPos);
		IMAGE_ADD_INTERNAL_CALL(GameplayComponent_GetPlayerID);
		IMAGE_ADD_INTERNAL_CALL(GameplayComponent_GetEntityIDByTag);
		IMAGE_ADD_INTERNAL_CALL(GameplayComponent_IsSwappable);
		IMAGE_ADD_INTERNAL_CALL(GameplayComponent_Swap);
		IMAGE_ADD_INTERNAL_CALL(GameplayComponent_SlowdownTime);

		IMAGE_ADD_INTERNAL_CALL(UIComponent_GetIsUIButtonClicked);
		IMAGE_ADD_INTERNAL_CALL(UIComponent_GetIsUIButtonHover);

		IMAGE_ADD_INTERNAL_CALL(SerializationComponent_GetIsFacingRight);
		IMAGE_ADD_INTERNAL_CALL(SerializationComponent_SetIsFacingRight);

		IMAGE_ADD_INTERNAL_CALL(EngineCore_GetMousePos);
		IMAGE_ADD_INTERNAL_CALL(EngineCore_PlayAudio);
		IMAGE_ADD_INTERNAL_CALL(EngineCore_LoadScene);
		IMAGE_ADD_INTERNAL_CALL(EngineCore_IsEditorMode);
		IMAGE_ADD_INTERNAL_CALL(EngineCore_SetText);
		IMAGE_ADD_INTERNAL_CALL(EngineCore_Quit);

		IMAGE_ADD_INTERNAL_CALL(PathfindingComponent_GetPath);

		IMAGE_ADD_INTERNAL_CALL(PhysicsComponent_GetRaycast);
		IMAGE_ADD_INTERNAL_CALL(PhysicsComponent_Collided);
		IMAGE_ADD_INTERNAL_CALL(PhysicsComponent_GetColliderDimensions);
		IMAGE_ADD_INTERNAL_CALL(PhysicsComponent_GetColliderPos);
		IMAGE_ADD_INTERNAL_CALL(PhysicsComponent_SetColliderPos);

		IMAGE_ADD_INTERNAL_CALL(AnimationComponent_GetAnimationState);
		IMAGE_ADD_INTERNAL_CALL(AnimationComponent_SetAnimationState);
		IMAGE_ADD_INTERNAL_CALL(GraphicsComponent_SetSprite);
		IMAGE_ADD_INTERNAL_CALL(GraphicsComponent_SetColour);
		IMAGE_ADD_INTERNAL_CALL(GraphicsComponent_GetScale);
		IMAGE_ADD_INTERNAL_CALL(GraphicsComponent_SetScale);
		IMAGE_ADD_INTERNAL_CALL(GraphicsComponent_GetRotation);
		IMAGE_ADD_INTERNAL_CALL(GraphicsComponent_SetRotation);

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
