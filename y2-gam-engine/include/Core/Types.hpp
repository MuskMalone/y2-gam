#pragma once

#include <bitset>
#include <cstdint>

// Source: https://gist.github.com/Lee-R/3839813
constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
{
	return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
}

constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count)
{
	return fnv1a_32(s, count);
}

constexpr std::uint64_t murmur64_seed(void const* data_, std::size_t len, std::size_t seed) {
#if defined(GB_ARCH_64_BIT)
    std::uint64_t const m = 0xc6a4a7935bd1e995ULL;
    std::int32_t const r = 47;

    std::uint64_t h = seed ^ (len * m);

    std::uint64_t const* data = (std::uint64_t const*)data_;
    std::uint8_t const* data2 = (std::uint8_t const*)data_;
    std::uint64_t const* end = data + (len / 8);

    while (data != end) {
        std::uint64_t k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    switch (len & 7) {
    case 7:
        h ^= (std::uint64_t)(data2[6]) << 48;
    case 6:
        h ^= (std::uint64_t)(data2[5]) << 40;
    case 5:
        h ^= (std::uint64_t)(data2[4]) << 32;
    case 4:
        h ^= (std::uint64_t)(data2[3]) << 24;
    case 3:
        h ^= (std::uint64_t)(data2[2]) << 16;
    case 2:
        h ^= (std::uint64_t)(data2[1]) << 8;
    case 1:
        h ^= (std::uint64_t)(data2[0]);
        h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
#else
    std::uint64_t h;
    std::uint32_t const m = 0x5bd1e995;
    std::int32_t const r = 24;

    std::uint32_t h1 = (std::uint32_t)(seed) ^ (std::uint32_t)(len);
    std::uint32_t h2 = (std::uint32_t)(seed >> 32);

    std::uint32_t const* data = (std::uint32_t const*)data_;

    while (len >= 8) {
        std::uint32_t k1, k2;
        k1 = *data++;
        k1 *= m;
        k1 ^= k1 >> r;
        k1 *= m;
        h1 *= m;
        h1 ^= k1;
        len -= 4;

        k2 = *data++;
        k2 *= m;
        k2 ^= k2 >> r;
        k2 *= m;
        h2 *= m;
        h2 ^= k2;
        len -= 4;
    }

    if (len >= 4) {
        std::uint32_t k1 = *data++;
        k1 *= m;
        k1 ^= k1 >> r;
        k1 *= m;
        h1 *= m;
        h1 ^= k1;
        len -= 4;
    }

    switch (len) {
    case 3:
        h2 ^= ((std::uint8_t const*)data)[2] << 16;
    case 2:
        h2 ^= ((std::uint8_t const*)data)[1] << 8;
    case 1:
        h2 ^= ((std::uint8_t const*)data)[0] << 0;
        h2 *= m;
    };

    h1 ^= h2 >> 18;
    h1 *= m;
    h2 ^= h1 >> 22;
    h2 *= m;
    h1 ^= h2 >> 17;
    h1 *= m;
    h2 ^= h1 >> 19;
    h2 *= m;

    h = h1;
    h = (h << 32) | h2;

    return h;
#endif
}
constexpr std::uint64_t murmur64(void const* data, std::size_t len) { return murmur64_seed(data, len, 0x9747b28c); }



// ECS
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 10000;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;


// Input
enum class MouseButtons
{
	LB, RB, MB
};


// Events
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
