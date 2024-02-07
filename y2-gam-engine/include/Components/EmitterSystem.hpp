#pragma once
#include <rapidjson/document.h>
#include <pch.hpp>
#pragma 
#define VCOUNT_POINT 1 //point
#define VCOUNT_LINE 2 //line
#define VCOUNT_QUAD 4 //quad
#define VCOUNT_ELLIPSE 5//circle

//type of emmission for all emitter types or points
#define EMT_TYPE_GRADUAL 0

//type of emissions for lines
#define EMT_TYPE_RAIN 1 //particles are fired with random angle wrt normal of line
#define EMT_TYPE_LAZER 2 //fires particles in a line in normal of line

//type of emissions for quads
#define EMT_TYPE_DUST 4
#define EMT_TYPE_DISINTEGRATE 5 // the tetricity block disintegrate

//presets for emitters
#define ALPHA_OVER_LIFETIME 0
#define SIZE_OVER_LIFETIME 1
#define ALPHA_SIZE_OVER_LIFETIME 2
#pragma warning(push)
#pragma warning(disable : 4324)
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
        int particlesPerFrame;  // 4 bytes

        bool alive;             // 1 byte (in practice, often treated as 4 bytes for alignment)
        char padding[3];        // 3 bytes of padding to align to 4 bytes

        // Additional padding at the end to align the entire structure size to a multiple of 16 bytes
        // If you add more data, make sure to calculate and adjust the padding as needed
        char endPadding[9];    // 9 bytes of padding to align the structure size to a multiple of 16 bytes
    };

    struct alignas(16) Particle {
        glm::vec4 col;          // vec4
        glm::vec4 pos;          // vec3
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
#pragma warning(pop)
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
    int particlesPerFrame; // 4 bytes
    int idx{-1};
    bool drawEmitterVertices{ false };
    // Padding might be added here to align the entire structure size
};
struct EmitterSystem
{
    using Emitters = std::vector<EmitterProxy>;
	EmitterSystem() = default;
	EmitterSystem([[maybe_unused]] rapidjson::Value const& obj) {

        for (size_t i{}; i < obj["emitters"].Size(); ++i) {
            EmitterProxy ep{};
            for (size_t j{}; j < obj["emitters"][static_cast<rapidjson::SizeType>(i)]["vertices"].Size(); ++j) {
                ep.vertices[j].x = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["vertices"][static_cast<rapidjson::SizeType>(j)]["x"].GetFloat();
				ep.vertices[j].y = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["vertices"][static_cast<rapidjson::SizeType>(j)]["y"].GetFloat();
				ep.vertices[j].z = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["vertices"][static_cast<rapidjson::SizeType>(j)]["z"].GetFloat();
				ep.vertices[j].w = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["vertices"][static_cast<rapidjson::SizeType>(j)]["w"].GetFloat();
            }
            ep.col.x = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["colX"].GetFloat();
            ep.col.y = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["colY"].GetFloat();
            ep.col.z = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["colZ"].GetFloat();
            ep.col.w = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["colW"].GetFloat();

            ep.gravity.x = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["gravityX"].GetFloat();
            ep.gravity.y = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["gravityY"].GetFloat();

            ep.size.x = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["sizeX"].GetFloat();
            ep.size.y = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["sizeY"].GetFloat();

            ep.rot = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["rot"].GetFloat();
            ep.lifetime = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["lifetime"].GetFloat();
            ep.angvel = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["angvel"].GetFloat();
            ep.speed = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["speed"].GetFloat();

            ep.frequency = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["frequency"].GetFloat();

            ep.type = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["type"].GetInt();
            ep.vCount = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["vCount"].GetInt();
            ep.preset = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["preset"].GetInt();
            ep.particlesPerFrame = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["particlesPerFrame"].GetInt();
            ep.drawEmitterVertices = obj["emitters"][static_cast<rapidjson::SizeType>(i)]["drawEmitterVertices"].GetBool();

            emitters.emplace_back(ep);
        }
    }
	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
        std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};

        //sm->InsertValue(obj, "assetID", assetID);
        Serializer::JSONObj emitterArr{JSON_ARR_TYPE};
        for (auto const& emitter : emitters) {
            Serializer::JSONObj emitterObj{JSON_OBJ_TYPE};
            Serializer::JSONObj vertices{JSON_ARR_TYPE};
            {
                for (size_t i{}; i < 4; ++i) {
                    Serializer::JSONObj vertex{JSON_OBJ_TYPE};
                    sm->InsertValue(vertex, "x", emitter.vertices[i].x);
                    sm->InsertValue(vertex, "y", emitter.vertices[i].y);
                    sm->InsertValue(vertex, "z", emitter.vertices[i].z);
                    sm->InsertValue(vertex, "w", emitter.vertices[i].w);
                    sm->PushToArray(vertices, vertex);
                }
                sm->InsertValue(emitterObj, "vertices", vertices);
            }
            sm->InsertValue(emitterObj, "colX", emitter.col.x);
            sm->InsertValue(emitterObj, "colY", emitter.col.y);
            sm->InsertValue(emitterObj, "colZ", emitter.col.z);
            sm->InsertValue(emitterObj, "colW", emitter.col.w);

            sm->InsertValue(emitterObj, "gravityX", emitter.gravity.x);
            sm->InsertValue(emitterObj, "gravityY", emitter.gravity.y);

            sm->InsertValue(emitterObj, "sizeX", emitter.size.x);
            sm->InsertValue(emitterObj, "sizeY", emitter.size.y);

            sm->InsertValue(emitterObj, "rot", emitter.rot);
            sm->InsertValue(emitterObj, "lifetime", emitter.lifetime);
            sm->InsertValue(emitterObj, "angvel", emitter.angvel);
            sm->InsertValue(emitterObj, "speed", emitter.speed);

            sm->InsertValue(emitterObj, "frequency", emitter.frequency);

            sm->InsertValue(emitterObj, "type", emitter.type);
            sm->InsertValue(emitterObj, "vCount", emitter.vCount);
            sm->InsertValue(emitterObj, "preset", emitter.preset);
            sm->InsertValue(emitterObj, "particlesPerFrame", emitter.particlesPerFrame);

            sm->InsertValue(emitterObj, "drawEmitterVertices", emitter.drawEmitterVertices);


            sm->PushToArray(emitterArr, emitterObj);
        }
        sm->InsertValue(obj, "emitters", emitterArr);
		return true;
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