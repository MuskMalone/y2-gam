#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       EventTypes.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief      unique has ids for events

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <cstdint>
// Events
// Source: https://gist.github.com/Lee-R/3839813
constexpr std::uint32_t fnv1a_32(char const* s, size_t count)
{
	return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
}

constexpr std::uint32_t operator "" _hash(char const* s, size_t count)
{
	return fnv1a_32(s, count);
}
inline std::uint32_t _hash(std::string const& s)
{
	return fnv1a_32(s.c_str(), s.size());
}
using EventId = std::uint32_t;

using ParamId = std::uint32_t;

#define METHOD_LISTENER(EventType, Listener) EventType, std::bind(&Listener, this, std::placeholders::_1)
#define FUNCTION_LISTENER(EventType, Listener) EventType, std::bind(&Listener, std::placeholders::_1)

// TODO: Make these easier to define and use (macro?)
// TODO: Add some kind of enforcement/automation that a SetParam type and a GetParam type match

namespace Events::Window {
	const EventId QUIT = "Events::Window::QUIT"_hash;
	const EventId RESIZED = "Events::Window::RESIZED"_hash;
	const EventId INPUT = "Events::Window::INPUT"_hash;
	const EventId TOGGLE_FULLSCREEN = "Events::Window::TOGGLE_FULLSCREEN"_hash;
}

namespace Events::Window::Input {
	const ParamId INPUT = "Events::Window::Input::INPUT"_hash;
	const ParamId KEY_CLICK = "Events::Window::Input::KEY_CLICK"_hash;
	const ParamId KEY_PRESS = "Events::Window::Input::KEY_PRESS"_hash;
	const ParamId KEY_RELEASE = "Events::Window::Input::KEY_RELEASE"_hash;

	const ParamId MOUSE_CLICK = "Events::Window::Input::MOUSE_CLICK"_hash;
	const ParamId MOUSE_PRESS = "Events::Window::Input::MOUSE_PRESS"_hash;
	const ParamId MOUSE_RELEASE = "Events::Window::Input::MOUSE_RELEASE"_hash;
	const ParamId MOUSE_MOVE = "Events::Window::Input::MOUSE_MOVE"_hash;
	const ParamId EDITOR_MOUSE_MOVE = "Events::Window::Input::EDITOR_MOUSE_MOVE"_hash;

}

namespace Events::Window::Resized {
	const ParamId WIDTH = "Events::Window::Resized::WIDTH"_hash;
	const ParamId HEIGHT = "Events::Window::Resized::HEIGHT"_hash;
}

namespace Events::System {
	const EventId ENTITY = "Events::System::ENTITY"_hash;
	const EventId SCENE = "Events::System::SCENE"_hash;
}
namespace Events::System::Scene {
	const EventId TRANSITION = "Events::System::Scene::TRANSITION"_hash;
}

namespace Events::System::Scene::Transition {
	const ParamId FROM_SCENE = "Events::System::Scene::Transition::FROM_SCENE"_hash;
	const ParamId TO_SCENE = "Events::System::Scene::Transition::TO_SCENE"_hash;
}
namespace Events::System::Entity {

	const ParamId CREATE = "Events::System::Entity::CREATE"_hash;
	const ParamId BEFORE_DESTROYED = "Events::System::Entity::BEFORE_DESTROYED"_hash;
#undef DELETE
	const ParamId DESTROYED = "Events::System::Entity::DESTROYED"_hash;
#define DELETE                           (0x00010000L)
	const ParamId COMPONENT_ADD = "Events::System::Entity::COMPONENT_ADD"_hash;
	const ParamId COMPONENT_REMOVE = "Events::System::Entity::COMPONENT_REMOVE"_hash;
	const ParamId BEFORE_COMPONENT_REMOVE = "Events::System::Entity::BEFORE_COMPONENT_REMOVE"_hash;
}

namespace Events::Physics {
	const EventId COLLISION = "Events::Physics::COLLISION"_hash;
}
namespace Events::Physics::Collision {
	const ParamId COLLIDED = "Events::Physics::Collision::COLLIDED"_hash;
}

namespace Events::Physics::Raycast {
	const EventId RAYCAST_FIRED = "Events::Physics::Raycast::RAYCAST_FIRED"_hash;
}

namespace Events::Physics::Raycast::Debug {
	const ParamId RAYCAST_DEBUGGED = "Events::Physics::Raycast::Debug::RAYCAST_DEBUGGED"_hash;
}

namespace Events::Particles {
	const EventId EMITTER = "Events::Particles::EMITTER"_hash;
}

namespace Events::Particles::Emitter {
	const ParamId EMITTER_ADDED = "Events::Particles::Emitter::EMITTER_ADDED"_hash;
	const ParamId BEFORE_EMITTER_DESTROY = "Events::Particles::Emitter::BEFORE_EMITTER_DESTROYED"_hash;
	const ParamId EMITTER_DESTROYED = "Events::Particles::Emitter::EMITTER_DESTROYED"_hash;
	const ParamId EMITTERPROXY_CHANGED = "Events::Particles::Emitter::EMITTERPROXY_CHANGED"_hash;
}