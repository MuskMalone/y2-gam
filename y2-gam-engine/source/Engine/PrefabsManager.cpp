/******************************************************************************/
/*!
\par        Image Engine
\file       PrefabsManager.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		prefab reading

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"

#include <Engine/PrefabsManager.hpp>
#include <Core/Serialization/SerializationManager.hpp>
#include <Core/Serialization/Serializer.hpp>
#include <Core/Coordinator.hpp>
#include <Core/Types.hpp>
#include "Scripting/ScriptManager.hpp"
#include <Core/Component.hpp>
std::shared_ptr<PrefabsManager> PrefabsManager::_mSelf = nullptr;
void PrefabsManager::Init() {
	std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
	std::shared_ptr<Coordinator> gCoordinator {Coordinator::GetInstance()};
	if (!sm->OpenJSON(cmPrefabsFilename)) return;
	if (!sm->At(cmPrefabsFilename, cmPrefabsFilename).IsObject()) return;
	for (rapidjson::Value::ConstMemberIterator itr = sm->At(cmPrefabsFilename, cmPrefabsFilename).MemberBegin(); itr != sm->At(cmPrefabsFilename, cmPrefabsFilename).MemberEnd(); ++itr) {
		std::cout << "Key: " << itr->name.GetString() << ", Value: ";
		Entity entity{ gCoordinator->CreateEntity() };
		if (itr->value.IsObject()) {
			for (rapidjson::Value::ConstMemberIterator innerItr = itr->value.MemberBegin(); innerItr != itr->value.MemberEnd(); ++innerItr) {
				auto at{ Serializer::gComponentSerializer.find(innerItr->name.GetString()) };
				std::cout << innerItr->name.GetString() << std::endl;
				if (at == Serializer::gComponentSerializer.end()) continue;
				at->second(entity, innerItr->value);
			}
		}
		PrefabID pid = _hash(itr->name.GetString());
		mPrefabsFactory[pid] = PrefabEntry{ itr->name.GetString(), pid, false, entity };
	}
}
void PrefabsManager::Exit() {
	
	std::shared_ptr< Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };
	sm->ClearJSON(cmPrefabsFilename);
	sm->SetObject(cmPrefabsFilename);
	using namespace Serializer;
	JSONObj prefabsObj{ JSON_OBJ_TYPE };
	for (auto const& prefab : mPrefabsFactory) {
		JSONObj obj{ JSON_OBJ_TYPE };
		SerializeEntity(prefab.second.entity, obj);
		if (!obj.ObjectEmpty())
			sm->InsertValue(prefabsObj, prefab.second.name.c_str(), obj);
	}

	sm->InsertValue(cmPrefabsFilename, prefabsObj);
	SerializationManager::GetInstance()->FlushJSON(cmPrefabsFilename);

}
void PrefabsManager::DeletePrefab(std::string const& name) {
	PrefabID id{ _hash(name) };
	DeletePrefab(id);
}
void PrefabsManager::DeletePrefab(PrefabID id){
	if (mPrefabsFactory.find(id) == mPrefabsFactory.end())
		return;
	Coordinator::GetInstance()->BlockEvent(Events::System::ENTITY);
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
	coordinator->DestroyEntity(mPrefabsFactory[id].entity);
	Image::ScriptManager::RemoveEntity(mPrefabsFactory[id].entity);
	mPrefabsFactory.erase(id);
	Coordinator::GetInstance()->UnblockEvent(Events::System::ENTITY);

}
Entity PrefabsManager::AddPrefab(std::string name) {
	PrefabID id{ _hash(name) };
	//tch: if name copied then make  anew one + copy at the end
	if (mPrefabsFactory.find(id) != mPrefabsFactory.end())
		return MAX_ENTITIES;
	Coordinator::GetInstance()->BlockEvent(Events::System::ENTITY);
	Entity entity{ Coordinator::GetInstance()->CreateEntity() };
	std::shared_ptr<Coordinator> coordinator{ Coordinator::GetInstance() };

	//Create the default components
	coordinator->AddComponent<Transform>(entity, Transform{
							{0.f,0.f,0.f},
							{0,0,0},
							{IMGUI_SCALE,IMGUI_SCALE,IMGUI_SCALE}
		}, true);
	coordinator->AddComponent<Layering>(entity, Layering{ LAYER_SENTINEL }, true);

	mPrefabsFactory[id] = std::move(PrefabEntry{
		name, id, false, entity
		});
	Coordinator::GetInstance()->UnblockEvent(Events::System::ENTITY);
	return entity;
}

//DO NOT USE IT REPEATEDLY IN FOR LOOP 
Entity PrefabsManager::SpawnPrefab(PrefabID key) {
	std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
	std::shared_ptr<Coordinator> gCoordinator {Coordinator::GetInstance()};
	using namespace Serializer;
	auto out = gCoordinator->CloneEntity(mPrefabsFactory[key].entity);
	if (gCoordinator->HasComponent<Script>(out)) {
		ScriptManager::OnCreateEntity(out);
	}
	gCoordinator->AddComponent<Prefab>(out, Prefab{static_cast<size_t>(key)});
	return out;
}
Entity PrefabsManager::SpawnPrefab(const char* key) {
	return SpawnPrefab(_hash(key));
}

Entity PrefabsManager::SpawnPrefab(PrefabID key, Vec2 const& pos) {
	Entity e{ SpawnPrefab(key) };
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
	if (coordinator->HasComponent<Transform>(e)) {
		auto& xfm{ coordinator->GetComponent<Transform>(e) };
		xfm.position.x = pos.x;
		xfm.position.y = pos.y;
	}
	if (coordinator->HasComponent<Collider>(e)) {
		auto& c{ coordinator->GetComponent<Collider>(e) };
		c.position += pos;
	}
	return e;
}

Entity PrefabsManager::SpawnPrefab(const char* key, Vec2 const& pos) {
	return (SpawnPrefab(_hash(key), pos));
}

void PrefabsSystem::Update() {
	std::shared_ptr<Coordinator> gCoordinator {Coordinator::GetInstance()};
	for (const auto& selectedEntity : mEntities) {
		Prefab& prefab = gCoordinator->GetComponent<Prefab>(selectedEntity);
		Entity prefabEntity = PrefabsManager::GetInstance()->GetEntityFactory().at(prefab.prefabId).entity;
        if (gCoordinator->HasComponent<Transform>(selectedEntity) && gCoordinator->HasComponent<Transform>(prefabEntity)){
            Transform& transform = gCoordinator->GetComponent<Transform>(selectedEntity);
			Transform& transform1 = gCoordinator->GetComponent<Transform>(prefabEntity);
			transform.scale = transform1.scale;
			transform.rotation = transform1.rotation;
			transform.elipse = transform1.elipse;
        }
        if (gCoordinator->HasComponent<Sprite>(selectedEntity) && gCoordinator->HasComponent<Sprite>(prefabEntity)) {
            Sprite& sprite = gCoordinator->GetComponent<Sprite>(selectedEntity);
			Sprite& sprite1 = gCoordinator->GetComponent<Sprite>(prefabEntity);

			sprite = sprite1;
        }
        if (gCoordinator->HasComponent<Animation>(selectedEntity) && gCoordinator->HasComponent<Animation>(prefabEntity)) {
			Animation& anim = gCoordinator->GetComponent<Animation>(selectedEntity);
			Animation& anim1 = gCoordinator->GetComponent<Animation>(prefabEntity);
			anim = anim1;
		}
        if (gCoordinator->HasComponent<Collider>(selectedEntity) && gCoordinator->HasComponent<Collider>(prefabEntity)) {
            Collider& collider = gCoordinator->GetComponent<Collider>(selectedEntity);
			Collider& collider1 = gCoordinator->GetComponent<Collider>(prefabEntity);
			collider.dimension = collider1.dimension;
			collider.rotation = collider1.rotation;
			collider.type = collider1.type;
			if (gCoordinator->HasComponent<Transform>(selectedEntity) && gCoordinator->HasComponent<Transform>(prefabEntity)) {
				Transform& transform = gCoordinator->GetComponent<Transform>(selectedEntity);
				collider.position = { collider.position.x + transform.position.x, collider.position.y + transform.position.y };
			}
        }
        if (gCoordinator->HasComponent<RigidBody>(selectedEntity) && gCoordinator->HasComponent<RigidBody>(prefabEntity)) {
            RigidBody& rigidBody = gCoordinator->GetComponent<RigidBody>(selectedEntity);
			RigidBody& rigidBody1 = gCoordinator->GetComponent<RigidBody>(prefabEntity);
			rigidBody = rigidBody1;
        }
        if (gCoordinator->HasComponent<Gravity>(selectedEntity) && gCoordinator->HasComponent<Gravity>(prefabEntity)) {
            Gravity& gravity = gCoordinator->GetComponent<Gravity>(selectedEntity);
			Gravity& gravity1 = gCoordinator->GetComponent<Gravity>(prefabEntity);
			gravity = gravity1;
        }
        if (gCoordinator->HasComponent<Text>(selectedEntity) && gCoordinator->HasComponent<Text>(prefabEntity)) {
            Text& text = gCoordinator->GetComponent<Text>(selectedEntity);
			Text& text1 = gCoordinator->GetComponent<Text>(prefabEntity);
			text = text1;
        }
        if (gCoordinator->HasComponent<Script>(selectedEntity) && gCoordinator->HasComponent<Script>(prefabEntity)) {
            Script& script = gCoordinator->GetComponent<Script>(selectedEntity);
			Script& script1 = gCoordinator->GetComponent<Script>(prefabEntity);
			if (script.name != script1.name) {
				ScriptManager::OnCreateEntity(selectedEntity);
			}
			script = script1;
        }
	}
	std::cout << mEntities.size() << " Prefab Entities spawned\n";
}
