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