#pragma once
#include <Core/Coordinator.hpp>
#include <Graphics/Shader.hpp>
class ParticleSystem : public System
{
    std::shared_ptr<Shader> mParticleShader;
    GLuint emitterSSbo;
    GLuint particleSSbo;
    GLuint particleCountSSbo;
public:

    void Init();
    void Update(float dt);
    void Destroy();
};
