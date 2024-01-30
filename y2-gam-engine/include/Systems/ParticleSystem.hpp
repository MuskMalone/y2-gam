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

    //for randomness in glsl
    GLuint mRandomSSbo;
    GLuint mRandomIdxSSbo;

    std::queue<GLuint> mEmitterIdxQueue;

    void EmitterAction(EmitterProxy & emitter, int action);
    void EventListener(Event&);
public:
    //1 for spawn, -1 for destroy, else edit
    static inline void AddEmitter(EmitterProxy const& e, Entity entity) {
        auto& emitter{ Coordinator::GetInstance()->GetComponent<EmitterSystem>(entity) };
        emitter.AddEmitter(e);
        Event event(Events::Particles::EMITTER);
        event.SetParam(Events::Particles::Emitter::EMITTER_ADDED, std::pair<int, Entity>(emitter.emitters.size() - 1, entity));
        Coordinator::GetInstance()->SendEvent(event);
    }
    static inline void RemoveEmitter(int idx, Entity entity) {
        auto& emitter{ Coordinator::GetInstance()->GetComponent<EmitterSystem>(entity) };
        if (idx >= emitter.emitters.size()) return;
        {
            Event event(Events::Particles::EMITTER);
            event.SetParam(Events::Particles::Emitter::BEFORE_EMITTER_DESTROY, std::pair<int, Entity>(idx, entity));
            Coordinator::GetInstance()->SendEvent(event);
        }
        emitter.RemoveEmitter(idx);
        {
            Event event(Events::Particles::EMITTER);
            event.SetParam(Events::Particles::Emitter::EMITTER_DESTROYED, std::pair<int, Entity>(idx, entity));
            Coordinator::GetInstance()->SendEvent(event);
        }
    }

    void Init();
    void Update(float dt);
    void DrawDebug();
    void Destroy();
};
