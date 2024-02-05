#include "../include/pch.hpp"
#include "Systems/ParticleSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Renderer.hpp"
#define MAX_BUFFER 5000000
#define WORK_GROUP 1000 //max buffer should be divisible by work group


void ParticleSystem::EventListener(Event& event) {
    auto coordinator = Coordinator::GetInstance();
    //if Emitter is added via gui
    {
        std::pair<int, Entity> e{ event.GetParam<std::pair<int, Entity>>(Events::Particles::Emitter::EMITTER_ADDED) };
        if (!event.GetFail()) {
            if (coordinator->HasComponent<EmitterSystem>(e.second)) {
                //add a new Emitter
                EmitterAction(coordinator->GetComponent<EmitterSystem>(e.second).emitters[e.first], 1);
            }
            return;
        }
    }

    //if emitter is added via serialization
    {
        Entity e{ event.GetParam<Entity>(Events::System::Entity::COMPONENT_ADD) };
        if (!event.GetFail()) {

            if (coordinator->HasComponent<EmitterSystem>(e)) {
                //add a new Emitter
                auto& emitters{ coordinator->GetComponent<EmitterSystem>(e).emitters };
                for (auto& emitter : emitters) {
                    EmitterAction(emitter, 1);
                }
            }
            return;
        }
    }

    //if emitter is destroyed
    {
        std::pair<int, Entity> e = event.GetParam<std::pair<int, Entity>>(Events::Particles::Emitter::BEFORE_EMITTER_DESTROY);
        if (!event.GetFail()) {
            if (coordinator->HasComponent<EmitterSystem>(e.second)) {
                EmitterAction(coordinator->GetComponent<EmitterSystem>(e.second).emitters[e.first], -1);
            }
            return;
        }
    }
    // destroy comes from system
    {
        Entity e{ event.GetParam<Entity>(Events::System::Entity::BEFORE_DESTROYED) };
        if (!event.GetFail()) {
            if (coordinator->HasComponent<EmitterSystem>(e)) {
                auto& emitters{ coordinator->GetComponent<EmitterSystem>(e).emitters };
                for (auto& emitter : emitters) {
                    EmitterAction(emitter, -1);
                }
            }
            return;
        }
    }

    //if emitter is changed
    {
        std::pair<int, Entity> e = event.GetParam<std::pair<int, Entity>>(Events::Particles::Emitter::EMITTERPROXY_CHANGED);
        if (!event.GetFail()) {
            if (coordinator->HasComponent<EmitterSystem>(e.second)) {
                EmitterAction(coordinator->GetComponent<EmitterSystem>(e.second).emitters[e.first], 0);
            }
            return;
        }
    }
}
void ParticleSystem::Init() {
    GLuint zero = 0;
    glGenBuffers(1, &mRandomIdxSSbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRandomIdxSSbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLuint), &zero);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &mParticleCountSSbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mParticleCountSSbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLuint), &zero);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    //I'm only going to comment one of these, because the other SSBOs are essentially the same
// Generate the initial buffer
    glGenBuffers(1, &mEmitterSSbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mEmitterSSbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BUFFER * sizeof(GLSLStructs::Emitter), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Do it again, twice.
    glGenBuffers(1, &mParticleSSbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mParticleSSbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BUFFER * sizeof(GLSLStructs::Particle), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    std::vector<float> randomData(MAX_BUFFER);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    for (size_t i = 0; i < MAX_BUFFER; ++i) {
        randomData[i] = dis(gen);
    }
    glGenBuffers(1, &mRandomSSbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRandomSSbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BUFFER * sizeof(float), NULL, GL_STATIC_DRAW);
    float* ptr = (float*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_BUFFER * sizeof(float), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    if (ptr) {
        // Copy the data to the SSBO
        std::memcpy(ptr, randomData.data(), MAX_BUFFER * sizeof(float));
        // Unmap the buffer
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }
    // Unbind the buffer (optional, for cleanup)
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 11, mRandomIdxSSbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 12, mRandomSSbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 13, mEmitterSSbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 14, mParticleSSbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 15, mParticleCountSSbo);
    // Ensures accesses to the SSBOs "reflect" writes from compute shader
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    mEmitterShader = std::make_shared<Shader>("../assets/shaders/Emitter.glsl");
    mEmitterStepShader = std::make_shared<Shader>("../assets/shaders/EmitterStep.glsl");
	mParticleShader = std::make_shared<Shader>("../assets/shaders/Particle.glsl");
    mParticleRenderShader = std::make_shared<Shader>("../assets/shaders/Particle.geom", "../assets/shaders/Particle.vert", "../assets/shaders/Particle.frag");

    for (uint64_t i{}; i < MAX_BUFFER; ++i) { mEmitterIdxQueue.push(i); }

    auto coordinator = Coordinator::GetInstance();

    //listens for events for particles and entity
    coordinator->AddEventListener(METHOD_LISTENER(Events::Particles::EMITTER, ParticleSystem::EventListener));
    coordinator->AddEventListener(METHOD_LISTENER(Events::System::ENTITY, ParticleSystem::EventListener));
}

void ParticleSystem::EmitterAction(EmitterProxy& emitter, int action) {
    mEmitterShader->Use();

    //gets the first available idx for emitter;
    if (action == 1) {
        emitter.idx = mEmitterIdxQueue.front();
        mEmitterIdxQueue.pop();
    }
    //push the available id back in the queue
    else if (action == -1) {
        mEmitterIdxQueue.push(emitter.idx);
    }
    mEmitterShader->SetUniform("spawnEmitter", action);
    mEmitterShader->SetUniform("emtTargetIdx", static_cast<GLint>(emitter.idx));
    
    if (action >= 0) {
        //sets the vertices
        GLint uEmtverticesLoc = glGetUniformLocation(mEmitterShader->PgmHdl(), "uEmtvertices");

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

        mEmitterShader->SetUniform("uEmtcol", emitter.col);

        mEmitterShader->SetUniform("uEmtgravity", emitter.gravity);
        mEmitterShader->SetUniform("uEmtsize", emitter.size);
        mEmitterShader->SetUniform("uEmtrot", emitter.rot);
        mEmitterShader->SetUniform("uEmtlifetime", emitter.lifetime);
        mEmitterShader->SetUniform("uEmtangvel", emitter.angvel);
        mEmitterShader->SetUniform("uEmtspeed", emitter.speed);

        mEmitterShader->SetUniform("uEmtfrequency", emitter.frequency);
        mEmitterShader->SetUniform("uEmttype", emitter.type);
        mEmitterShader->SetUniform("uEmtvCount", emitter.vCount);
        mEmitterShader->SetUniform("uEmtpreset", emitter.preset);
        mEmitterShader->SetUniform("uEmtparticlesPerFrame", emitter.particlesPerFrame);
    }

    glDispatchCompute(1, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    //set the flags back
    //mParticleShader->SetUniform("spawnEmitter", 0);
    mEmitterShader->SetUniform("emtTargetIdx", -1);

    mEmitterShader->Unuse();
}

void ParticleSystem::Update(float dt) {
    auto inputSystem = Coordinator::GetInstance()->GetSystem<InputSystem>();

    static float timeElapsed = 0.f;
    timeElapsed += dt;

    mEmitterStepShader->Use();

    mEmitterStepShader->SetUniform("DT", dt);
    mEmitterStepShader->SetUniform("bufferMaxCount", MAX_BUFFER);
    //1000 is the number of elements per grp
    glDispatchCompute(MAX_BUFFER / WORK_GROUP, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    mEmitterStepShader->Unuse();

    mParticleShader->Use();
	mParticleShader->SetUniform("DT", dt);
	//mParticleShader->SetUniform("bufferMaxCount", MAX_BUFFER);
	//1000 is the number of elements per grp
	glDispatchCompute(MAX_BUFFER / WORK_GROUP, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    mParticleShader->Unuse();

    //Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetFramebuffer(0)->Bind();
    //Draw();
    //Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetFramebuffer(0)->Unbind();
    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, mEmitterSSbo);
    //GLSLStructs::Emitter* vels = (GLSLStructs::Emitter*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_BUFFER * sizeof(GLSLStructs::Emitter), GL_MAP_READ_BIT);
    //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, mParticleSSbo);
    //GLSLStructs::Particle* vels = (GLSLStructs::Particle*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_BUFFER * sizeof(GLSLStructs::Particle), GL_MAP_READ_BIT);
    //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRandomSSbo);
    //float* vels = (float*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_BUFFER * sizeof(float), GL_MAP_READ_BIT);
    //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRandomIdxSSbo);
    //GLuint* idx = (GLuint*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT);
    //std::cout << *idx << "partrandidx\n";
    //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, mParticleCountSSbo);
    //GLuint* idx = (GLuint*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT);
    //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}
void ParticleSystem::Draw() {
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    mParticleRenderShader->Use();
    auto& cam{ Coordinator::GetInstance()->GetComponent<Camera>(Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetCamera()) };

    glm::mat4 viewprojection{ cam.GetViewProjMtx() };
    mParticleRenderShader->SetUniform("vertViewProjection", viewprojection);
    glDrawArrays(GL_POINTS, 0, MAX_BUFFER);
	mParticleRenderShader->Unuse();
}
void ParticleSystem::DrawDebug() {
    for (auto const& e : mEntities) {
        auto const& emitterSystem{ Coordinator::GetInstance()->GetComponent<EmitterSystem>(e) };
        for (auto const& emitter : emitterSystem.emitters) {
            if (emitter.drawEmitterVertices) {
                switch (emitter.vCount) {
                case 1: {
                    Renderer::DrawCircle({ emitter.vertices[0].x,emitter.vertices[0].y, 0.f }, { 5, 5 }, emitter.col);
                } break;
                case 2: {
                    Renderer::DrawCircle({ emitter.vertices[0].x,emitter.vertices[0].y, 0.f }, { 5, 5 }, emitter.col);
                    Renderer::DrawCircle({ emitter.vertices[1].x,emitter.vertices[1].y, 0.f }, { 5, 5 }, emitter.col);
                    Renderer::DrawLine(emitter.vertices[0], emitter.vertices[1], emitter.col);
                } break;
                case 4: {
                    Renderer::DrawCircle({ emitter.vertices[0].x,emitter.vertices[0].y, 0.f }, { 5, 5 }, emitter.col);
                    Renderer::DrawLine(emitter.vertices[0], emitter.vertices[1], emitter.col);
                    Renderer::DrawCircle({ emitter.vertices[1].x,emitter.vertices[1].y, 0.f }, { 5, 5 }, emitter.col);
                    Renderer::DrawLine(emitter.vertices[1], emitter.vertices[2], emitter.col);
                    Renderer::DrawCircle({ emitter.vertices[2].x,emitter.vertices[2].y, 0.f }, { 5, 5 }, emitter.col);
                    Renderer::DrawLine(emitter.vertices[2], emitter.vertices[3], emitter.col);
                    Renderer::DrawCircle({ emitter.vertices[3].x,emitter.vertices[3].y, 0.f }, { 5, 5 }, emitter.col);
                    Renderer::DrawLine(emitter.vertices[3], emitter.vertices[0], emitter.col);
                }break;
                }

            }
        }
    }
}
void ParticleSystem::Destroy() {
    glDeleteBuffers(1, &mRandomIdxSSbo);
    glDeleteBuffers(1, &mRandomSSbo);
    glDeleteBuffers(1, &mEmitterSSbo);
    glDeleteBuffers(1, &mParticleSSbo);
    glDeleteBuffers(1, &mParticleCountSSbo);
}