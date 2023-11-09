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
	if (!sm->OpenJSON("Prefabs")) return;

}


//DO NOT USE IT REPEATEDLY IN FOR LOOP 
// IT IS SLOW AS HELL
Entity PrefabsManager::SpawnPrefab(const char* key) {
	std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
	std::shared_ptr<Coordinator> gCoordinator {Coordinator::GetInstance()};
	using namespace Serializer;
	if (!sm->At("Prefabs", "Prefabs").IsObject()) return static_cast<Entity>(-1);
	if (!sm->At("Prefabs", "Prefabs")[key].IsObject()) return static_cast<Entity>( -1);
	Entity entity{ gCoordinator->CreateEntity() };
	for (auto itr = (sm->At("Prefabs")[key]).MemberBegin(); itr != (sm->At("Prefabs")[key]).MemberEnd(); ++itr) {
		auto at{ gComponentSerializer.find(itr->name.GetString()) };
		if (at == gComponentSerializer.end()) continue;
		at->second(entity, itr->value);
	}
	return entity;
}