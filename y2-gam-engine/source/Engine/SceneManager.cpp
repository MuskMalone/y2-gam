#include "../include/pch.hpp"
#include <Engine/SceneManager.hpp>
#include <Engine/AssetManager.hpp>
#include <Core/Coordinator.hpp>
#include <Systems/EntitySerializationSystem.hpp>

namespace {
	static const char* prefix = "scenes/";
	static const char* assetsKey = "Assets";
	static const char* lvlKey = "Entities";
	std::shared_ptr<Coordinator> gCoordinator = Coordinator::GetInstance();
}

std::shared_ptr<SceneManager> SceneManager::_mSelf = 0;
std::shared_ptr<SceneManager> SceneManager::GetInstance() {
	if (!_mSelf) return _mSelf = std::make_shared<SceneManager>();
	return _mSelf;
}

void SceneManager::Init() {
	::gCoordinator = Coordinator::GetInstance();
	::gCoordinator->AddEventListener(METHOD_LISTENER(Events::System::ENTITY, SceneManager::OnEntityEvent));
	mSceneListenerStart = false;
}

void SceneManager::LoadScene(std::string const& scnpath) {
	using namespace Serializer;
	if (!mCurrentScene.empty()) {
		ExitScene(mCurrentScene);
	}
	mCurrentScene = scnpath;
	const auto filename{ prefix + scnpath };
	mSceneListenerStart = true;
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
	auto sm{ Serializer::SerializationManager::GetInstance()};
	if (sm->OpenJSON(filename)) {
		Serializer::JSONObj& aidsJson{ sm->At(filename, assetsKey) };
			std::set<AssetID> aids{};
			if (aidsJson.IsArray()) {
				for (size_t i{}; i < aidsJson.Size(); ++i) {
					aids.insert(aidsJson[static_cast<rapidjson::SizeType>(i)].GetUint64());
				}
			}
		AssetManager::GetInstance()->LoadAssetChunk(aids);
			coordinator->GetSystem<Serializer::EntitySerializationSystem>()->LoadEntities(sm->At(filename, lvlKey));
	}
	else { // if started from an empty obj
		sm->SetObject(filename);
		JSONObj assetlist{ JSON_ARR_TYPE };
		sm->InsertValue(assetsKey, assetlist);
		JSONObj entlist{ JSON_ARR_TYPE };
		sm->InsertValue(lvlKey, entlist);
	}
}

void SceneManager::ModifyScene(std::string const& scnpath) {
	//im just updateing the entities for now.
	const auto filename{ prefix + scnpath };
	using namespace Serializer;
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
	std::shared_ptr< Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };
	sm->GetDoc(filename).RemoveMember(lvlKey);
	JSONObj entArr{ JSON_ARR_TYPE };
	coordinator->GetSystem<Serializer::EntitySerializationSystem>()->FlushEntities(entArr);
	sm->InsertValue(lvlKey, entArr);

}

void SceneManager::SaveScene(std::string const& scnpath) {
	const auto filename{ prefix + scnpath };
	ModifyScene(scnpath);
	Serializer::SerializationManager::GetInstance()->FlushJSON(filename);
}
void SceneManager::ExitScene(std::string const& scnpath) {
	mSceneListenerStart = false;
	mCurrentScene.clear();
	auto coordinator{ Coordinator::GetInstance() };

	for (auto const& e : mEntities) {
		coordinator->DestroyEntity(e);
	}
	mEntities.clear();
}
void SceneManager::ResetScene(std::string const& scnpath) {

	ExitScene(scnpath);
	LoadScene(scnpath);
}

void SceneManager::AddAsset(std::string const& scnpath,AssetID aid) {
	const auto filename{ prefix + scnpath };
	using namespace Serializer;
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
	std::shared_ptr< Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };
	JSONObj& assetlist{ sm->At(filename, assetsKey) };
	// Iterate through the array

	if (assetlist.IsArray()) {
		for (rapidjson::SizeType i = 0; i < assetlist.Size(); i++) {
			// Check the id of each object
			if (assetlist[i].GetUint64() == aid) {
				// Remove the object
				assetlist.Erase(assetlist.Begin() + i);
				return; // if there is an existing asset remove it
			}
		}
		sm->PushToArray(assetlist, aid);
	}
}

void SceneManager::RemoveAsset(std::string const& scnpath, AssetID aid) {
	const auto filename{ prefix + scnpath };
	using namespace Serializer;
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
	std::shared_ptr< Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };
	JSONObj& assetlist{ sm->At(filename, assetsKey) };
	if (assetlist.IsArray()) {
		// Iterate through the array
		for (rapidjson::SizeType i = 0; i < assetlist.Size(); i++) {
			// Check the id of each object
			if (assetlist[i].GetUint64() == aid) {
				// Remove the object
				assetlist.Erase(assetlist.Begin() + i);
				break; // Assuming only one object with id = 2, break after removing it
			}
		}
	}
}

void SceneManager::ModifyScene() {
	if (IsSceneActive())
		ModifyScene(mCurrentScene);
}
void SceneManager::SaveScene() {
	if (IsSceneActive())
	SaveScene(mCurrentScene);
}
void SceneManager::ExitScene() {
	if (IsSceneActive())
	ExitScene(mCurrentScene);
}
void SceneManager::ResetScene() {
	if (IsSceneActive()){
		std::string scenestr {mCurrentScene};
		ResetScene(scenestr);
	}
}
void SceneManager::AddAsset(AssetID aid) {
	if (IsSceneActive())
	AddAsset(mCurrentScene, aid);
}
void SceneManager::RemoveAsset(AssetID aid) {
	if (IsSceneActive())
	RemoveAsset(mCurrentScene, aid);
}

void SceneManager::OnEntityEvent(Event& event) {
	if (!mSceneListenerStart) return;
	auto e = event.GetParam<Entity>(Events::System::Entity::CREATE);
	if (!event.GetFail()) {
		mEntities.insert(e);
	}
	e = event.GetParam<Entity>(Events::System::Entity::DESTROYED);
	if (!event.GetFail()) {
		mEntities.erase(e);
	}
}