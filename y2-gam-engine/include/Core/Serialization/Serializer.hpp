#pragma once
#include <Core/Types.hpp>
#include <functional>
#include <Core/Coordinator.hpp>
#include <rapidjson/document.h>
#include <string>
#include <map>
#include <Core/Serialization/SerializationManager.hpp>
#include <Components/Animation.hpp>
#include <Components/Camera.hpp>
#include <Components/Collider.hpp>
#include <Components/Editor.hpp>
#include <Components/Gravity.hpp>
#include <Components/OrthoCamera.hpp>
#include <Components/RigidBody.hpp>
#include <Components/Script.hpp>
#include <Components/Sprite.hpp>
#include <Components/Tag.hpp>
#include <Components/Transform.hpp>
namespace Serializer{
static void EntityAddAnimation(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Animation{ obj });
}
static void EntityAddCamera(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Camera{ obj });
}
static void EntityAddCollider(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Collider{ obj });
}
static void EntityAddEditor(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Editor{ obj });
}
static void EntityAddGravity(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Gravity{ obj });
}
static void EntityAddOrthoCamera(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, OrthoCamera{ obj });
}
static void EntityAddRigidBody(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, RigidBody{ obj });
}
static void EntityAddScript(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Script{ obj });
}
static void EntityAddSprite(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Sprite{ obj });
}
static void EntityAddTag(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Tag{ obj });
}
static void EntityAddTransform(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Transform{ obj });
}
template <typename _type> std::string TypeToString() {
if constexpr (std::is_same_v<_type, Animation>) return "Animation";
else if constexpr (std::is_same_v<_type, Camera>) return "Camera";
else if constexpr (std::is_same_v<_type, Collider>) return "Collider";
else if constexpr (std::is_same_v<_type, Editor>) return "Editor";
else if constexpr (std::is_same_v<_type, Gravity>) return "Gravity";
else if constexpr (std::is_same_v<_type, OrthoCamera>) return "OrthoCamera";
else if constexpr (std::is_same_v<_type, RigidBody>) return "RigidBody";
else if constexpr (std::is_same_v<_type, Script>) return "Script";
else if constexpr (std::is_same_v<_type, Sprite>) return "Sprite";
else if constexpr (std::is_same_v<_type, Tag>) return "Tag";
else if constexpr (std::is_same_v<_type, Transform>) return "Transform";
else return "NULL";
}
static void SerializeEntity(Entity const& entity, JSONObj& ent) {
if (Coordinator::GetInstance()->HasComponent<Animation>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Animation>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Animation>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<Camera>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Camera>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Camera>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<Collider>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Collider>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Collider>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<Editor>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Editor>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Editor>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<Gravity>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Gravity>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Gravity>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<OrthoCamera>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<OrthoCamera>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<OrthoCamera>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<RigidBody>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<RigidBody>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<RigidBody>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<Script>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Script>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Script>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<Sprite>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Sprite>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Sprite>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<Tag>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Tag>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Tag>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<Transform>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Transform>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Transform>(), obj); }
else { obj.SetNull(); }
}
}
static const std::map<std::string, std::function<void(Entity const&, rapidjson::Value const&)>> gComponentSerializer{
{"Animation", EntityAddAnimation},
{"Camera", EntityAddCamera},
{"Collider", EntityAddCollider},
{"Editor", EntityAddEditor},
{"Gravity", EntityAddGravity},
{"OrthoCamera", EntityAddOrthoCamera},
{"RigidBody", EntityAddRigidBody},
{"Script", EntityAddScript},
{"Sprite", EntityAddSprite},
{"Tag", EntityAddTag},
{"Transform", EntityAddTransform}
};
}
