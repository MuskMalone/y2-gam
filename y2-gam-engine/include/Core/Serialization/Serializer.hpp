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
#include <Components/Layering.hpp>
#include <Components/Node.hpp>
#include <Components/Prefab.hpp>
#include <Components/RigidBody.hpp>
#include <Components/Script.hpp>
#include <Components/Sprite.hpp>
#include <Components/Tag.hpp>
#include <Components/Text.hpp>
#include <Components/Transform.hpp>
#include <Components/UIImage.hpp>
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
static void EntityAddLayering(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Layering{ obj });
}
static void EntityAddNode(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Node{ obj });
}
static void EntityAddPrefab(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Prefab{ obj });
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
static void EntityAddText(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Text{ obj });
}
static void EntityAddTransform(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Transform{ obj });
}
static void EntityAddUIImage(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, UIImage{ obj });
}
template <typename _type> std::string TypeToString() {
if constexpr (std::is_same_v<_type, Animation>) return "Animation";
else if constexpr (std::is_same_v<_type, Camera>) return "Camera";
else if constexpr (std::is_same_v<_type, Collider>) return "Collider";
else if constexpr (std::is_same_v<_type, Editor>) return "Editor";
else if constexpr (std::is_same_v<_type, Gravity>) return "Gravity";
else if constexpr (std::is_same_v<_type, Layering>) return "Layering";
else if constexpr (std::is_same_v<_type, Node>) return "Node";
else if constexpr (std::is_same_v<_type, Prefab>) return "Prefab";
else if constexpr (std::is_same_v<_type, RigidBody>) return "RigidBody";
else if constexpr (std::is_same_v<_type, Script>) return "Script";
else if constexpr (std::is_same_v<_type, Sprite>) return "Sprite";
else if constexpr (std::is_same_v<_type, Tag>) return "Tag";
else if constexpr (std::is_same_v<_type, Text>) return "Text";
else if constexpr (std::is_same_v<_type, Transform>) return "Transform";
else if constexpr (std::is_same_v<_type, UIImage>) return "UIImage";
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
if (Coordinator::GetInstance()->HasComponent<Layering>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Layering>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Layering>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<Node>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Node>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Node>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<Prefab>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Prefab>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Prefab>(), obj); }
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
if (Coordinator::GetInstance()->HasComponent<Text>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Text>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Text>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<Transform>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Transform>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Transform>(), obj); }
else { obj.SetNull(); }
}
if (Coordinator::GetInstance()->HasComponent<UIImage>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<UIImage>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<UIImage>(), obj); }
else { obj.SetNull(); }
}
}
static const std::map<std::string, std::function<void(Entity const&, rapidjson::Value const&)>> gComponentSerializer{
{"Animation", EntityAddAnimation},
{"Camera", EntityAddCamera},
{"Collider", EntityAddCollider},
{"Editor", EntityAddEditor},
{"Gravity", EntityAddGravity},
{"Layering", EntityAddLayering},
{"Node", EntityAddNode},
{"Prefab", EntityAddPrefab},
{"RigidBody", EntityAddRigidBody},
{"Script", EntityAddScript},
{"Sprite", EntityAddSprite},
{"Tag", EntityAddTag},
{"Text", EntityAddText},
{"Transform", EntityAddTransform},
{"UIImage", EntityAddUIImage}
};
}
