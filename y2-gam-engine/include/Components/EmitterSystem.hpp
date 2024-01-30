#pragma once
#include <rapidjson/document.h>
#include <pch.hpp>
#define EMT_TYPE_SMOKE 0
#define EMT_TYPE_FIRE 1
#define EMT_TYPE_BURST 2
#define EMT_TYPE_BURSTGRAV 3
#define EMT_TYPE_GRADUAL 4

#define EMT_SHAPE_POINT 1
#define EMT_SHAPE_LINE 2
#define EMT_SHAPE_RECT 3
namespace GLSLStructs {

    //1-1 map of the structs in Particle.glsl
    struct alignas(16) Emitter {
        glm::vec4 vertices[4];  // 4 vec4s, each vec4 is 16 bytes, total 64 bytes
        glm::vec4 col;          // 16 bytes

        glm::vec2 gravity;      // 8 bytes
        glm::vec2 size;         // 8 bytes
        float rot;              // 4 bytes
        float lifetime;         // 4 bytes
        float angvel;           // 4 bytes
        float speed;            // 4 bytes

        float time;             // 4 bytes
        float frequency;        // 4 bytes

        int type;               // 4 bytes
        int vCount;             // 4 bytes
        int preset;             // 4 bytes

        bool alive;             // 1 byte (in practice, often treated as 4 bytes for alignment)
        char padding[3];        // 3 bytes of padding to align to 4 bytes

        // Additional padding at the end to align the entire structure size to a multiple of 16 bytes
        // If you add more data, make sure to calculate and adjust the padding as needed
        char endPadding[12];    // 12 bytes of padding to align the structure size to a multiple of 16 bytes
    };

    struct alignas(16) Particle {
        glm::vec4 col;          // vec4
        glm::vec3 pos;          // vec3
        glm::vec2 vel;          // vec2
        glm::vec2 gravity;
        glm::vec2 size;         // vec2
        float rot;
        float age;
        float lifetime;
        float angvel;
        int emtIdx;
        bool alive;
    };

}
struct EmitterProxy {
    glm::vec4 vertices[4]; // Each vec4 is 16 bytes, total 64 bytes
    glm::vec4 col;         // 16 bytes (vec3 is aligned like vec4)

    glm::vec2 gravity; // 8 bytes
    glm::vec2 size;    // 8 bytes (vec2 is aligned to 8 bytes)
    float rot;    // 4 bytes
    float lifetime; // 4 bytes
    float angvel;  // 4 bytes
    float speed;     // 8 bytes

    float time;       // 4 bytes, but due to the vec3 above, you can expect padding here
    float frequency;  // 4 bytes

    // type of emmission
    //0: smoke
    //1: fire
    //2: burst
    //3: burst with gravity
    //4: gradual emission
    int type;         // 4 bytes
    // 1 for point, 2 for line, 4 for rect
    int vCount{ 1 };       // 4 bytes
    int preset;    // 4 bytes //alpha over lifetime etc
    int idx;
    bool drawEmitterVertices{ false };
    // Padding might be added here to align the entire structure size
};
struct EmitterSystem
{
    using Emitters = std::vector<EmitterProxy>;
	EmitterSystem() = default;
	EmitterSystem([[maybe_unused]] rapidjson::Value const& obj) {}
	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		return false;
	}
    //do not use
    inline void AddEmitter(EmitterProxy const& e) { emitters.push_back(e); }
    //do not use
    inline void RemoveEmitter(int idx) { if (idx < emitters.size()) emitters.erase(emitters.begin() + idx); }
    //do not use
    Emitters const& GetEmitters() { return emitters; }
    //do not use
    Emitters emitters;

};