#include "../include/pch.hpp"
#include "Systems/ParticleSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Renderer.hpp"
#define MAX_BUFFER 5000000

namespace {
	
	//1-1 map of the structs in Particle.glsl
    struct alignas(16) Emitter {
        glm::vec4 vertices[4]; // 4 vec4s, each vec4 is 4 floats
        glm::vec3 pos;          // vec3
        float padding0;        // Padding for alignment
        float time;
        float frequency;
        int type;
        int vCount;
        bool alive;
        char padding1[3];      // Padding to align to 16 bytes
    };

    struct alignas(16) Particle {
        glm::vec4 col;          // vec4
        glm::vec3 pos;          // vec3
        float padding0;        // Padding for alignment
        glm::vec2 vel;          // vec2
        glm::vec2 size;         // vec2
        float rot;
        float age;
        float lifetime;
        float angvel;
        bool alive;
        char padding1[3];      // Padding to align to 16 bytes
    };

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
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BUFFER * sizeof(Emitter), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
 //   // Set the bitmask that OpenGL will actually use when copying data to buffer
 //   // This particular bitmask tells opengl to write to the buffer, and that previous contents can be thrown away
 //   GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

 //   // positions and velocities generated randomly in a sphere
 //   // glMapBufferRange actually lets us stream this data to graphics card memory
 //   Emitter* points = (Emitter*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_BUFFER * sizeof(Emitter), bufMask);
	//for (int i = 0; i < MAX_BUFFER; i++)
	//{

	//}
    // unmap the buffer (break stream) now that we've uploaded the data
    //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    // Do it again, twice.
    glGenBuffers(1, &mParticleSSbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mParticleSSbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BUFFER * sizeof(Particle), NULL, GL_STATIC_DRAW);
 //   Particle* particles = (Particle*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_BUFFER * sizeof(Particle), bufMask);
	//for (int i = 0; i < MAX_BUFFER; i++)
	//{
	//	
	//}
    //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 13, mEmitterSSbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 14, mParticleSSbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 15, mParticleCountSSbo);
    // Ensures accesses to the SSBOs "reflect" writes from compute shader
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	mParticleShader = std::make_shared<Shader>("../assets/shaders/Particle.glsl");
    mParticleRenderShader = std::make_shared<Shader>("../assets/shaders/Particle.geom", "../assets/shaders/Particle.vert", "../assets/shaders/Particle.frag");

}
void ParticleSystem::Update(float dt) {
    static float timeElapsed = 0.f;
    timeElapsed += dt;
    mParticleShader->Use();

	mParticleShader->SetUniform("DT", dt);
	mParticleShader->SetUniform("bufferMaxCount", MAX_BUFFER);
	//1000 is the number of elements per grp
	glDispatchCompute(MAX_BUFFER / 1000, 1, 1);
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