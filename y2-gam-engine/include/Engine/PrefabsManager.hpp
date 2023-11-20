#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       PrefabsManager.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		for reading prefabs

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include <Core/Types.hpp>
#include <Core/Globals.hpp>
struct PrefabEntry;
class PrefabsManager {
public:
	using PrefabID = uint64_t;

	using PrefabEntities = std::map<PrefabID, PrefabEntry>;

	void Init();
	void Exit();
	Entity AddPrefab(std::string name, Entity e);
	Entity SpawnPrefab(const char* key);
	Entity SpawnPrefab(PrefabID id);
	static std::shared_ptr<PrefabsManager> GetInstance() {
		if (!_mSelf) return _mSelf = std::make_shared<PrefabsManager>();
		return _mSelf;
	}
	PrefabEntities const& GetEntityFactory() { return mPrefabsFactory; }

private:

	const char* cmPrefabsFilename{ "Prefabs" };
	PrefabEntities mPrefabsFactory;
	static std::shared_ptr<PrefabsManager> _mSelf;
};
struct PrefabEntry {
	std::string name;
	PrefabsManager::PrefabID id;
	bool isChanged;
	Entity entity;
};