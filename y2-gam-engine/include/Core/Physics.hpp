#pragma once
#include <Math/Vec2.hpp>
#include <cstdint>
#include <Components/RigidBody.hpp>
#include <Core/Types.hpp>
#include <unordered_map>

namespace Physics {
    constexpr float PI{ 3.14159265358979323846f };
    enum class Axis { FACE_A_X, FACE_A_Y, FACE_B_X, FACE_B_Y };

    enum class EdgeNumbers {
        NO_EDGE = 0, EDGE1, EDGE2, EDGE3, EDGE4

    };
    union FeaturePair {
        struct Edges {
            EdgeNumbers in_edge_1;
            EdgeNumbers out_edge_1;
            EdgeNumbers in_edge_2;
            EdgeNumbers out_edge_2;
        } e;
        uint32_t value;
    };

    struct ClipVertex {
        Vec2 v;
        FeaturePair fp;
    };

    constexpr uint64_t MAX_CONTACT_POINTS{ 2 };
    struct Contact {
        Vec2 position;
        Vec2 normal;
        Vec2 r1, r2;
        float seperation;
        float acc_normal_impulse;
        float acc_tangent_impulse;
        float acc_biased_normal_impulse;
        float mass_normal;
        float mass_tangent;
        float bias;
        FeaturePair feature;
    };
    struct ArbiterKey {
        Entity b1;
        Entity b2;
    };
    struct Arbiter {
        Entity b1;
        Entity b2;
        //RigidBody* b1; //use of pointers are unfortunate but.....idc
        //RigidBody* b2;
        float combined_friction;

        Contact contacts[MAX_CONTACT_POINTS];
        uint32_t contacts_count;
    };
    using ArbiterHashTable = std::unordered_map<size_t, Arbiter>;

}