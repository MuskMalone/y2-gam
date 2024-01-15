#pragma once
#include <Core/Coordinator.hpp>
#include <Graphics/Shader.hpp>
class ParticleSystem : public System
{
    std::shared_ptr<Shader> mParticleShader;
    std::shared_ptr<Shader> mParticleRenderShader;
    GLuint mEmitterSSbo;
    GLuint mParticleSSbo;
    GLuint mParticleCountSSbo;
public:

    void Init();
    void Update(float dt);
    void Destroy();
};
