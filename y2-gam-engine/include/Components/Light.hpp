#pragma once

#include "Math/MathUtils.h"
#include <Core/Serialization/SerializationManager.hpp>
#include <Math/MathUtils.h>
#include <rapidjson/document.h>
namespace GLSLStructs {
    
    struct alignas(8) Point {
        glm::vec2 pos;
        float angle;
    };
    struct alignas(8) Intersect {
        glm::vec2 pos;
        float param;
    };
    struct alignas(8) Ray {
        glm::vec2 start;
        glm::vec2 end;
    };
    struct alignas(16) Light {
        glm::vec4 intensity;
        glm::vec2 pos;
        glm::vec2 dir;
        float angle; // angle for viewing
        float radius;
        int entityId;
        int verticesDrawn;
    };
    struct alignas(8) Block {
        glm::vec2 dim;
        glm::vec2 pos;
        float rot;
        int entityId;
    };
}
struct Light
{
	Vec2 force;
	Light() = default;
	Light(Vec2 const& v) : force{ v } {}
	Light(rapidjson::Value const& obj) {
	}
	bool Serialize(rapidjson::Value& obj) {
		return false;

	}
};
