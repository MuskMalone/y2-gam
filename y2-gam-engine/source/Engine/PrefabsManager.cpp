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

Entity PrefabsManager::AddPrefab(std::string name) {
	PrefabID id{ _hash(name) };
	//tch: if name copied then make  anew one + copy at the end
	if (mPrefabsFactory.find(id) != mPrefabsFactory.end()) 
		return MAX_ENTITIES;
	Coordinator::GetInstance()->BlockEvent(Events::System::ENTITY);
	Entity entity{ Coordinator::GetInstance()->CreateEntity() };
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};

	//Create the default components
	coordinator->AddComponent<Transform>(entity, Transform{});
	coordinator->AddComponent<Layering>(entity, Layering{ "" });

	mPrefabsFactory[id] = std::move(PrefabEntry{
		name, id, false, entity
	});
	Coordinator::GetInstance()->UnblockEvent(Events::System::ENTITY);
	return entity;
}

//DO NOT USE IT REPEATEDLY IN FOR LOOP 
// IT IS SLOW AS FUCK I WILL FUCKING SLAP YOU
Entity PrefabsManager::SpawnPrefab(PrefabID key) {
	std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
	std::shared_ptr<Coordinator> gCoordinator {Coordinator::GetInstance()};
	using namespace Serializer;
	return gCoordinator->CloneEntity(mPrefabsFactory[key].entity);
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

