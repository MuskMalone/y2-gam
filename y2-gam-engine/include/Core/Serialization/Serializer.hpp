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
#include <Components/Emitter.hpp>
#include <Components/Gravity.hpp>
#include <Components/Layering.hpp>
#include <Components/Node.hpp>
#include <Components/Prefab.hpp>
#include <Components/RigidBody.hpp>
#include <Components/Script.hpp>
#include <Components/Sprite.hpp>
#include <Components/Swappable.hpp>
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
static void EntityAddEmitter(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Emitter{ obj });
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
static void EntityAddSwappable(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Swappable{ obj });
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
else if constexpr (std::is_same_v<_type, Emitter>) return "Emitter";
else if constexpr (std::is_same_v<_type, Gravity>) return "Gravity";
else if constexpr (std::is_same_v<_type, Layering>) return "Layering";
else if constexpr (std::is_same_v<_type, Node>) return "Node";
else if constexpr (std::is_same_v<_type, Prefab>) return "Prefab";
else if constexpr (std::is_same_v<_type, RigidBody>) return "RigidBody";
else if constexpr (std::is_same_v<_type, Script>) return "Script";
else if constexpr (std::is_same_v<_type, Sprite>) return "Sprite";
else if constexpr (std::is_same_v<_type, Swappable>) return "Swappable";
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
if (Coordinator::GetInstance()->HasComponent<Emitter>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Emitter>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Emitter>(), obj); }
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
if (Coordinator::GetInstance()->HasComponent<Swappable>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
bool res = Coordinator::GetInstance()->GetComponent<Swappable>(entity).Serialize(obj);
if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Swappable>(), obj); }
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
{"Emitter", EntityAddEmitter},
{"Gravity", EntityAddGravity},
{"Layering", EntityAddLayering},
{"Node", EntityAddNode},
{"Prefab", EntityAddPrefab},
{"RigidBody", EntityAddRigidBody},
{"Script", EntityAddScript},
{"Sprite", EntityAddSprite},
{"Swappable", EntityAddSwappable},
{"Tag", EntityAddTag},
{"Text", EntityAddText},
{"Transform", EntityAddTransform},
{"UIImage", EntityAddUIImage}
};
static std::map<std::string, std::any> SaveEntities(Entity entity) {
std::map<std::string, std::any> out{};
if (Coordinator::GetInstance()->HasComponent<Animation>(entity)) { out["Animation"] = std::any{ Coordinator::GetInstance()->GetComponent<Animation>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Camera>(entity)) { out["Camera"] = std::any{ Coordinator::GetInstance()->GetComponent<Camera>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Collider>(entity)) { out["Collider"] = std::any{ Coordinator::GetInstance()->GetComponent<Collider>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Editor>(entity)) { out["Editor"] = std::any{ Coordinator::GetInstance()->GetComponent<Editor>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Emitter>(entity)) { out["Emitter"] = std::any{ Coordinator::GetInstance()->GetComponent<Emitter>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Gravity>(entity)) { out["Gravity"] = std::any{ Coordinator::GetInstance()->GetComponent<Gravity>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Layering>(entity)) { out["Layering"] = std::any{ Coordinator::GetInstance()->GetComponent<Layering>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Node>(entity)) { out["Node"] = std::any{ Coordinator::GetInstance()->GetComponent<Node>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Prefab>(entity)) { out["Prefab"] = std::any{ Coordinator::GetInstance()->GetComponent<Prefab>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<RigidBody>(entity)) { out["RigidBody"] = std::any{ Coordinator::GetInstance()->GetComponent<RigidBody>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Script>(entity)) { out["Script"] = std::any{ Coordinator::GetInstance()->GetComponent<Script>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Sprite>(entity)) { out["Sprite"] = std::any{ Coordinator::GetInstance()->GetComponent<Sprite>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Swappable>(entity)) { out["Swappable"] = std::any{ Coordinator::GetInstance()->GetComponent<Swappable>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Tag>(entity)) { out["Tag"] = std::any{ Coordinator::GetInstance()->GetComponent<Tag>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Text>(entity)) { out["Text"] = std::any{ Coordinator::GetInstance()->GetComponent<Text>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<Transform>(entity)) { out["Transform"] = std::any{ Coordinator::GetInstance()->GetComponent<Transform>(entity) }; }
if (Coordinator::GetInstance()->HasComponent<UIImage>(entity)) { out["UIImage"] = std::any{ Coordinator::GetInstance()->GetComponent<UIImage>(entity) }; }
return out;
}
static Entity UndestroyEntity(std::map<std::string, std::any> const& components) {
Entity e{ Coordinator::GetInstance()->CreateEntity() };
for (auto const& c : components) {
if (c.first == "Animation") { Coordinator::GetInstance()->AddComponent<Animation>(e, std::any_cast<Animation>(c.second)); }
if (c.first == "Camera") { Coordinator::GetInstance()->AddComponent<Camera>(e, std::any_cast<Camera>(c.second)); }
if (c.first == "Collider") { Coordinator::GetInstance()->AddComponent<Collider>(e, std::any_cast<Collider>(c.second)); }
if (c.first == "Editor") { Coordinator::GetInstance()->AddComponent<Editor>(e, std::any_cast<Editor>(c.second)); }
if (c.first == "Emitter") { Coordinator::GetInstance()->AddComponent<Emitter>(e, std::any_cast<Emitter>(c.second)); }
if (c.first == "Gravity") { Coordinator::GetInstance()->AddComponent<Gravity>(e, std::any_cast<Gravity>(c.second)); }
if (c.first == "Layering") { Coordinator::GetInstance()->AddComponent<Layering>(e, std::any_cast<Layering>(c.second)); }
if (c.first == "Node") { Coordinator::GetInstance()->AddComponent<Node>(e, std::any_cast<Node>(c.second)); }
if (c.first == "Prefab") { Coordinator::GetInstance()->AddComponent<Prefab>(e, std::any_cast<Prefab>(c.second)); }
if (c.first == "RigidBody") { Coordinator::GetInstance()->AddComponent<RigidBody>(e, std::any_cast<RigidBody>(c.second)); }
if (c.first == "Script") { Coordinator::GetInstance()->AddComponent<Script>(e, std::any_cast<Script>(c.second)); }
if (c.first == "Sprite") { Coordinator::GetInstance()->AddComponent<Sprite>(e, std::any_cast<Sprite>(c.second)); }
if (c.first == "Swappable") { Coordinator::GetInstance()->AddComponent<Swappable>(e, std::any_cast<Swappable>(c.second)); }
if (c.first == "Tag") { Coordinator::GetInstance()->AddComponent<Tag>(e, std::any_cast<Tag>(c.second)); }
if (c.first == "Text") { Coordinator::GetInstance()->AddComponent<Text>(e, std::any_cast<Text>(c.second)); }
if (c.first == "Transform") { Coordinator::GetInstance()->AddComponent<Transform>(e, std::any_cast<Transform>(c.second)); }
if (c.first == "UIImage") { Coordinator::GetInstance()->AddComponent<UIImage>(e, std::any_cast<UIImage>(c.second)); }
}
return e;
}
}
