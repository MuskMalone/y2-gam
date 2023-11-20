#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       Physics.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief      physics data types

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Math/MathUtils.h"
#include <cstdint>
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
            EdgeNumbers inEdge1;
            EdgeNumbers outEdge1;
            EdgeNumbers inEdge2;
            EdgeNumbers outEdge2;
        } e;
        uint32_t value{};
    };

    struct ClipVertex {
        Vec2 v{};
        FeaturePair fp{};
    };

    constexpr uint64_t MAX_CONTACT_POINTS{ 2 };
    struct Contact {
        Vec2 position{};
        Vec2 normal{};
        Vec2 r1{}, r2{};
        float seperation{};
        float accNormalImpulse{};
        float accTangentImpulse{};
        float accBiasedNormalImpulse{};
        float massNormal{};
        float massTangent{};
        float bias{};
        FeaturePair feature{};
    };
    struct ArbiterKey {
        Entity b1{};
        Entity b2{};
    };
    struct Arbiter {
        //entity that is querying the collision
        Entity b1{};

        //entity that has been collided with;
        Entity b2{};
        float combinedFriction{};

        Contact contacts[MAX_CONTACT_POINTS];
        uint32_t contactsCount{};
    };
    struct RayHit {
        Vec2 normal;
        Vec2 point;
        float distance;
        Entity entityID;
    };

    using ArbiterVec = std::vector<Arbiter>;
    using ArbiterHashTable = std::unordered_map<size_t, Arbiter>;
    using ArbiterPair = std::pair<size_t, Arbiter>;

    //for phys system. returns entities that are collided and resolved by physics
    bool IsCollided(Entity const& e1, Entity const& e2);
    bool IsCollided(Entity const& e1, Entity const& e2, Arbiter &a);
    //this is kinda inefficient
    ArbiterVec IsCollided(Entity const& e1);

    //for collisionsystem. returns entities whose colliders intersect.
    // physics resolution not guaranteed for this method
    bool IsIntersected(Entity const& e1, Entity const& e2);
    bool IsIntersected(Entity const& e1, Entity const& e2, Arbiter &a);
    ArbiterVec IsIntersected(Entity const& e1);
}
