#pragma once
#include <Core/Types.hpp>
#include <functional>
#include <Core/Coordinator.hpp>
#include <rapidjson/document.h>
#include <string>
#include <map>
#include <Core/Serialization/SerializationManager.hpp>
#include <Components/Animation.hpp>
#include <Components/BoxCollider.hpp>
#include <Components/Camera.hpp>
#include <Components/Editor.hpp>
#include <Components/Gravity.hpp>
#include <Components/RigidBody.hpp>
#include <Components/Sprite.hpp>
#include <Components/Transform.hpp>
namespace Serializer{
static void EntityAddAnimation(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Animation{ obj });
}
static void EntityAddBoxCollider(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, BoxCollider{ obj });
}
static void EntityAddCamera(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Camera{ obj });
}
static void EntityAddEditor(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Editor{ obj });
}
static void EntityAddGravity(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Gravity{ obj });
}
static void EntityAddRigidBody(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, RigidBody{ obj });
}
static void EntityAddSprite(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Sprite{ obj });
}
static void EntityAddTransform(Entity const& entity, rapidjson::Value const& obj) {
Coordinator::GetInstance()->AddComponent(entity, Transform{ obj });
}
template <typename _type> std::string TypeToString() {
if constexpr (std::is_same_v<_type, Animation>) return "Animation";
if constexpr (std::is_same_v<_type, BoxCollider>) return "BoxCollider";
if constexpr (std::is_same_v<_type, Camera>) return "Camera";
if constexpr (std::is_same_v<_type, Editor>) return "Editor";
if constexpr (std::is_same_v<_type, Gravity>) return "Gravity";
if constexpr (std::is_same_v<_type, RigidBody>) return "RigidBody";
if constexpr (std::is_same_v<_type, Sprite>) return "Sprite";
if constexpr (std::is_same_v<_type, Transform>) return "Transform";
return "NULL";
}
static void SerializeEntity(Entity const& entity, JSONObj& ent) {
if (Coordinator::GetInstance()->HasComponent<Animation>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
Coordinator::GetInstance()->GetComponent<Animation>(entity).Serialize(obj);
SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Animation>(), obj);
}
if (Coordinator::GetInstance()->HasComponent<BoxCollider>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
Coordinator::GetInstance()->GetComponent<BoxCollider>(entity).Serialize(obj);
SerializationManager::GetInstance()->InsertValue(ent, TypeToString<BoxCollider>(), obj);
}
if (Coordinator::GetInstance()->HasComponent<Camera>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
Coordinator::GetInstance()->GetComponent<Camera>(entity).Serialize(obj);
SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Camera>(), obj);
}
if (Coordinator::GetInstance()->HasComponent<Editor>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
Coordinator::GetInstance()->GetComponent<Editor>(entity).Serialize(obj);
SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Editor>(), obj);
}
if (Coordinator::GetInstance()->HasComponent<Gravity>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
Coordinator::GetInstance()->GetComponent<Gravity>(entity).Serialize(obj);
SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Gravity>(), obj);
}
if (Coordinator::GetInstance()->HasComponent<RigidBody>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
Coordinator::GetInstance()->GetComponent<RigidBody>(entity).Serialize(obj);
SerializationManager::GetInstance()->InsertValue(ent, TypeToString<RigidBody>(), obj);
}
if (Coordinator::GetInstance()->HasComponent<Sprite>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
Coordinator::GetInstance()->GetComponent<Sprite>(entity).Serialize(obj);
SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Sprite>(), obj);
}
if (Coordinator::GetInstance()->HasComponent<Transform>(entity)){
JSONObj obj{ JSON_OBJ_TYPE };
obj.SetObject();
Coordinator::GetInstance()->GetComponent<Transform>(entity).Serialize(obj);
SerializationManager::GetInstance()->InsertValue(ent, TypeToString<Transform>(), obj);
}
}
static const std::map<std::string, std::function<void(Entity const&, rapidjson::Value const&)>> gComponentSerializer{
{"Animation", EntityAddAnimation},
{"BoxCollider", EntityAddBoxCollider},
{"Camera", EntityAddCamera},
{"Editor", EntityAddEditor},
{"Gravity", EntityAddGravity},
{"RigidBody", EntityAddRigidBody},
{"Sprite", EntityAddSprite},
{"Transform", EntityAddTransform}
};
}
