#pragma once
#include <Core/Coordinator.hpp>
#include <Graphics/Shader.hpp>
#include <Core/Component.hpp>
class ParticleSystem : public System
{
    std::shared_ptr<Shader> mParticleShader;
    std::shared_ptr<Shader> mParticleRenderShader;
    GLuint mEmitterSSbo;
    GLuint mParticleSSbo;
    GLuint mParticleCountSSbo;

    std::queue<GLuint> mEmitterIdxQueue;

    //1 for spawn, -1 for destroy, else edit
    void EmitterAction(Emitter & emitter, int action);
    void EventListener(Event&);
public:

    void Init();
    void Update(float dt);
    void Destroy();
};
