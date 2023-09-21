#pragma once
#include <Core/Types.hpp>
#include <functional>
#include <Core/Coordinator.hpp>
#include <rapidjson/document.h>
#include <string>
#include <map>
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
if constexpr (std::is_same_v<T, Animation>) return "Animation";
if constexpr (std::is_same_v<T, BoxCollider>) return "BoxCollider";
if constexpr (std::is_same_v<T, Camera>) return "Camera";
if constexpr (std::is_same_v<T, Editor>) return "Editor";
if constexpr (std::is_same_v<T, Gravity>) return "Gravity";
if constexpr (std::is_same_v<T, RigidBody>) return "RigidBody";
if constexpr (std::is_same_v<T, Sprite>) return "Sprite";
if constexpr (std::is_same_v<T, Transform>) return "Transform";
return "NULL";
}
static void SerializeEntity(Entity const& entity, rapidjson::Value& obj) {
if (Coordinator::GetInstance()->HasComponent<Animation>(entity))
Coordinator::GetInstance()->GetComponent<Animation>(entity).Serialize(obj);
if (Coordinator::GetInstance()->HasComponent<BoxCollider>(entity))
Coordinator::GetInstance()->GetComponent<BoxCollider>(entity).Serialize(obj);
if (Coordinator::GetInstance()->HasComponent<Camera>(entity))
Coordinator::GetInstance()->GetComponent<Camera>(entity).Serialize(obj);
if (Coordinator::GetInstance()->HasComponent<Editor>(entity))
Coordinator::GetInstance()->GetComponent<Editor>(entity).Serialize(obj);
if (Coordinator::GetInstance()->HasComponent<Gravity>(entity))
Coordinator::GetInstance()->GetComponent<Gravity>(entity).Serialize(obj);
if (Coordinator::GetInstance()->HasComponent<RigidBody>(entity))
Coordinator::GetInstance()->GetComponent<RigidBody>(entity).Serialize(obj);
if (Coordinator::GetInstance()->HasComponent<Sprite>(entity))
Coordinator::GetInstance()->GetComponent<Sprite>(entity).Serialize(obj);
if (Coordinator::GetInstance()->HasComponent<Transform>(entity))
Coordinator::GetInstance()->GetComponent<Transform>(entity).Serialize(obj);
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
