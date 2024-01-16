#include "../include/pch.hpp"
#include "Systems/ParticleSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Renderer.hpp"
#define MAX_BUFFER 5000000
#define WORK_GROUP 1000 //max buffer should be divisible by work group

namespace GLSLStructs {
	
	//1-1 map of the structs in Particle.glsl
    struct alignas(16) Emitter {
        glm::vec4 vertices[4]; // 4 vec4s, each vec4 is 4 floats
        glm::vec4 color;          // vec3
        glm::vec2 gravity;
        float time;
        float frequency;
        int type;
        int vCount;
        bool alive;
        char padding1[7];      // Padding to align to 16 bytes
    };

    struct alignas(16) Particle {
        glm::vec4 col;          // vec4
        glm::vec3 pos;          // vec3
        float padding0;        // Padding for alignment
        glm::vec2 vel;          // vec2
        glm::vec2 gravity;
        glm::vec2 size;         // vec2
        float rot;
        float age;
        float lifetime;
        float angvel;
        bool alive;
        char padding1[3];      // Padding to align to 16 bytes
    };

}
void ParticleSystem::EventListener(Event& event) {
    auto coordinator = Coordinator::GetInstance();
    //if Emitter is added
    Entity e{ event.GetParam<Entity>(Events::System::Entity::COMPONENT_ADD) };
    if (event.GetFail()) return;
    if (coordinator->HasComponent<Emitter>(e)) {
        //add a new Emitter
        EmitterAction(coordinator->GetComponent<Emitter>(e), 1);
    }

    //if emitter is destroyed
    e = event.GetParam<Entity>(Events::System::Entity::BEFORE_COMPONENT_REMOVE);
    if (event.GetFail()) return;
    if (coordinator->HasComponent<Emitter>(e)) {
        EmitterAction(coordinator->GetComponent<Emitter>(e), -1);
    }

    //if emitter is destroyed
    e = event.GetParam<Entity>(Events::System::Entity::BEFORE_DESTROYED);
    if (event.GetFail()) return;
    if (coordinator->HasComponent<Emitter>(e)) {
        EmitterAction(coordinator->GetComponent<Emitter>(e), -1);
    }
}
void ParticleSystem::Init() {
	glGenBuffers(1, &mParticleCountSSbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mParticleCountSSbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);

	// Initialize particleIdx to 0
	GLuint zero = 0;
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLuint), &zero);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    //I'm only going to comment one of these, because the other SSBOs are essentially the same
// Generate the initial buffer
    glGenBuffers(1, &mEmitterSSbo);
    // Bind to graphics card memory
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mEmitterSSbo);
    // Allocate necessary storage 
    // This might also be able to dump data at the same time. Needs testing though.
    // If it ain't broke, don't fix it
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BUFFER * sizeof(GLSLStructs::Emitter), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    // Do it again, twice.
    glGenBuffers(1, &mParticleSSbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mParticleSSbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BUFFER * sizeof(GLSLStructs::Particle), NULL, GL_STATIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 13, mEmitterSSbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 14, mParticleSSbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 15, mParticleCountSSbo);
    // Ensures accesses to the SSBOs "reflect" writes from compute shader
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	mParticleShader = std::make_shared<Shader>("../assets/shaders/Particle.glsl");
    mParticleRenderShader = std::make_shared<Shader>("../assets/shaders/Particle.geom", "../assets/shaders/Particle.vert", "../assets/shaders/Particle.frag");

    for (uint64_t i{}; i < MAX_BUFFER; ++i) { mEmitterIdxQueue.push(i); }

    auto coordinator = Coordinator::GetInstance();
    coordinator->AddEventListener(METHOD_LISTENER(Events::System::ENTITY, ParticleSystem::EventListener));
}

void ParticleSystem::EmitterAction(Emitter & emitter, int action) {
    mParticleShader->Use();

    //gets the first available idx for emitter;
    if (action == 1) {
        emitter.EmitterIdx() = mEmitterIdxQueue.front();
        mEmitterIdxQueue.pop();
    }
    //push the available id back in the queue
    else if (action == -1) {
        mEmitterIdxQueue.push(emitter.EmitterIdx());
    }

    mParticleShader->SetUniform("spawnEmitter", action);
    mParticleShader->SetUniform("emtTargetIdx", static_cast<GLint>(emitter.EmitterIdx()));
    
    if (action >= 0) {
        //sets the vertices
        GLint uEmtverticesLoc = glGetUniformLocation(mParticleShader->PgmHdl(), "uEmtvertices");

        //sets the emitter shape
        auto const& v{ emitter.vertices };
        GLfloat vertices[16] = {
            // vec4 1
            v[0][0], v[0][1], v[0][2], v[0][3],
            // vec4 2
            v[1][0], v[1][1], v[1][2], v[1][3],
            // vec4 3
            v[2][0], v[2][1], v[2][2], v[2][3],
            // vec4 4
            v[3][0], v[3][1], v[3][2], v[3][3],
        };
        glUniform4fv(uEmtverticesLoc, 4, vertices);

        mParticleShader->SetUniform("uEmtcol", emitter.color);
        mParticleShader->SetUniform("uEmtgravity", emitter.gravity);
        mParticleShader->SetUniform("uEmtfrequency", emitter.frequency);
        mParticleShader->SetUniform("uEmttype", emitter.type);
        mParticleShader->SetUniform("uEmtvCount", emitter.vCount);
    }

    glDispatchCompute(MAX_BUFFER / WORK_GROUP, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    //set the flags back
    //mParticleShader->SetUniform("spawnEmitter", 0);
    mParticleShader->SetUniform("emtTargetIdx", -1);

    mParticleShader->Unuse();
}

void ParticleSystem::Update(float dt) {
    static float timeElapsed = 0.f;
    timeElapsed += dt;
    mParticleShader->Use();

    mParticleShader->SetUniform("uTimeElapsed", timeElapsed);
	mParticleShader->SetUniform("DT", dt);
	mParticleShader->SetUniform("bufferMaxCount", MAX_BUFFER);
	//1000 is the number of elements per grp
	glDispatchCompute(MAX_BUFFER / WORK_GROUP, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    mParticleShader->Unuse();
    Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetFramebuffer(0)->Bind();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    mParticleRenderShader->Use();
    auto& cam{ Coordinator::GetInstance()->GetComponent<Camera>(Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetCamera()) };
    glm::mat4 projection{ cam.GetProjMtx() };
    mParticleRenderShader->SetUniform("vertProjection", projection);
    glDrawArrays(GL_POINTS, 0, MAX_BUFFER);
    Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetFramebuffer(0)->Unbind();
    mParticleRenderShader->Unuse();
}

void ParticleSystem::Destroy() {

}