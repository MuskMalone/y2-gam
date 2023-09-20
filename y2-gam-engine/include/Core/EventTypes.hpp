#pragma once
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
}

namespace Events::Window::Resized {
	const ParamId WIDTH = "Events::Window::Resized::WIDTH"_hash;
	const ParamId HEIGHT = "Events::Window::Resized::HEIGHT"_hash;
}

namespace Events::Physics {
	const EventId COLLISION = "Events::Physics::COLLISION"_hash;
}
namespace Events::Physics::Collision {
	const ParamId COLLIDED = "Events::Physics::Collision::COLLIDED"_hash;
}