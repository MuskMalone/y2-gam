#pragma once
#include <memory>
#include <map>
#include <string>
#include <Core/Types.hpp>
#include <Core/Globals.hpp>
class PrefabsManager {
public:
	void Init();
	Entity SpawnPrefab(const char* key);
	static std::shared_ptr<PrefabsManager> GetInstance() {
		if (!_mSelf) return _mSelf = std::make_shared<PrefabsManager>();
		return _mSelf;
	}

private:
	static std::shared_ptr<PrefabsManager> _mSelf;
};