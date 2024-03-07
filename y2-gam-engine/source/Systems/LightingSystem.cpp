/******************************************************************************/
/*!
\par        Image Engine
\file       LightingSystem.cpp

\author     tan cheng hian t.chenghian
\date       Nov 11, 2023

\brief      The header file for the particle systme

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
                        or disclosure of this file or its contents without the prior
                        written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "Systems/LightingSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Engine/SceneManager.hpp"
#include "Core/Component.hpp"
#define MAX_BUFFER MAX_ENTITIES
#define WORK_GROUP 100 //max buffer should be divisible by work group

/**
 * @brief Event listener for the particle system.
 *
 * This function handles various events related to particle emitters and entities.
 * It performs actions based on the type of event received, such as adding or destroying emitters.
 *
 * @param event The event object containing the event data.
 */

void LightingSystem::EventListener(Event& event) {
    auto coordinator = Coordinator::GetInstance();
}

/**
 * @brief Initializes the particle system.
 *
 * This function initializes the particle system by creating the necessary buffers and shaders.
 */
void LightingSystem::Init() {
//    GLuint zero = 0;
//    glGenBuffers(1, &mRandomIdxSSbo);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRandomIdxSSbo);
//    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
//    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLuint), &zero);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//
//    glGenBuffers(1, &mParticleCountSSbo);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mParticleCountSSbo);
//    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
//    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(GLuint), &zero);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//
//    //I'm only going to comment one of these, because the other SSBOs are essentially the same
//// Generate the initial buffer
//    glGenBuffers(1, &mEmitterSSbo);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mEmitterSSbo);
//    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BUFFER * sizeof(GLSLStructs::Emitter), NULL, GL_STATIC_DRAW);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//
//    // Do it again, twice.
//    glGenBuffers(1, &mParticleSSbo);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mParticleSSbo);
//    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BUFFER * sizeof(GLSLStructs::Particle), NULL, GL_STATIC_DRAW);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//
//    glGenBuffers(1, &mParticleStartSSbo);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mParticleStartSSbo);
//    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BUFFER * sizeof(GLSLStructs::Particle), NULL, GL_STATIC_DRAW);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//
//    GLuint texHdlSSbo;
//    glGenBuffers(1, &texHdlSSbo);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, texHdlSSbo);
//    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BUFFER * sizeof(GLSLStructs::TextureData), NULL, GL_STATIC_DRAW);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//
//    std::vector<float> randomData(MAX_BUFFER);
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
//    for (size_t i = 0; i < MAX_BUFFER; ++i) {
//        randomData[i] = dis(gen);
//    }
//    glGenBuffers(1, &mRandomSSbo);
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mRandomSSbo);
//    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_BUFFER * sizeof(float), NULL, GL_STATIC_DRAW);
//    float* ptr = (float*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_BUFFER * sizeof(float), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
//    if (ptr) {
//        // Copy the data to the SSBO
//        std::memcpy(ptr, randomData.data(), MAX_BUFFER * sizeof(float));
//        // Unmap the buffer
//        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
//    }
//    // Unbind the buffer (optional, for cleanup)
//    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 9, texHdlSSbo);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 10, mParticleStartSSbo);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 11, mRandomIdxSSbo);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 12, mRandomSSbo);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 13, mEmitterSSbo);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 14, mParticleSSbo);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 15, mParticleCountSSbo);
//    // Ensures accesses to the SSBOs "reflect" writes from compute shader
//    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
//
//    mEmitterShader = std::make_shared<Shader>("../assets/shaders/Emitter.glsl");
//    mEmitterStepShader = std::make_shared<Shader>("../assets/shaders/EmitterStep.glsl");
//    mParticleShader = std::make_shared<Shader>("../assets/shaders/Particle.glsl");
//    mParticleRenderShader = std::make_shared<Shader>("../assets/shaders/Particle.geom", "../assets/shaders/Particle.vert", "../assets/shaders/Particle.frag");
//
//    for (uint64_t i{}; i < MAX_BUFFER; ++i) { mEmitterIdxQueue.push(static_cast<unsigned int>(i)); }
//
//    auto coordinator = Coordinator::GetInstance();
//
//    //listens for events for particles and entity
//    coordinator->AddEventListener(METHOD_LISTENER(Events::Particles::EMITTER, LightingSystem::EventListener));
//    coordinator->AddEventListener(METHOD_LISTENER(Events::System::ENTITY, LightingSystem::EventListener));
}

/**
 * @brief Updates the particle system.
 *
 * This function updates the particle system by updating the particles and emitters.
 *
 * @param dt The time elapsed since the last update.
 */
void LightingSystem::Update(float dt) {
    intersects.clear();
    std::vector<Point> points{};
    std::vector<Ray> segments{};
    auto inputSystem = Coordinator::GetInstance()->GetSystem<InputSystem>();
    //for testing, arbitrary
    glm::vec2 point = {inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second };
    for (auto const& entity : mEntities) {
        auto const& collider = Coordinator::GetInstance()->GetComponent<Collider>(entity);
        // Assuming Vec2 is equivalent to glm::vec2
        glm::vec2 dimension = glm::vec2{collider.dimension.x, collider.dimension.y}; // Example dimensions
        glm::vec2 position = glm::vec2{collider.position.x, collider.position.y};  // Example position
        float rotation = collider.rotation; // Example rotation in radians

        // Start with an identity matrix
        glm::mat4 transformationMatrix = glm::mat4(1.0f);

        // Apply translation
        auto transMatrix = glm::translate(transformationMatrix, glm::vec3(position, 0.0f));

        // Apply rotation around the Z-axis
        auto rotMatrix = glm::rotate(transformationMatrix, rotation, glm::vec3(0.0f, 0.0f, 1.0f));

        // Apply scaling
        auto scaleMatrix = glm::scale(transformationMatrix, glm::vec3(dimension, 1.0f));

        transformationMatrix = transMatrix * rotMatrix * scaleMatrix;

        std::vector<glm::vec2> squarePoints = {
            glm::vec2(-0.5f, -0.5f),
            glm::vec2(-0.5f,  0.5f),
            glm::vec2(0.5f,   0.5f),
            glm::vec2(0.5f,  -0.5f)
        };

        for (auto& p : squarePoints) {
            // Convert glm::vec2 to glm::vec4, setting z to 0, and w to 1 for the transformation
            glm::vec4 pointVec4 = glm::vec4(p, 0.0f, 1.0f);

            // Apply the transformation
            glm::vec4 transformedVec4 = transformationMatrix * pointVec4;

            // Convert back to glm::vec2 and store it
            points.push_back({ p = glm::vec2(transformedVec4), std::atan2(transformedVec4.y - point.y, transformedVec4.x - point.x) });
        }
        for (int i{}; i < 4; ++i) {
            if (i != 3)
                segments.push_back(Ray{ squarePoints[i], squarePoints[i + 1] });
            else
                segments.push_back(Ray{ squarePoints[3], squarePoints[0] });
        }
    }
    for (auto const& p : points) {
        std::array<float, 3> angles{
            p.angle - 0.00001f, p.angle, p.angle + 0.00001
        };
        for (auto const& a : angles) {
            float angle = a;

            // Calculate dx & dy from angle
            auto dx = glm::cos(angle);
            auto dy = glm::sin(angle);

            // Ray from center of screen to mouse
            auto ray = Ray{
                point,
                {point.x + dx, point.y + dy}
            };

            // Find CLOSEST intersection
            Intersect closestIntersect = { point, std::numeric_limits<float>::max() };
            float t{ 0 };
            for (auto i = 0; i < segments.size(); i++) {
                Intersect intersect{};
                if (!getIntersection(ray, segments[i], intersect)) continue;

                if (intersect.param < closestIntersect.param) {
                    closestIntersect = intersect;
                }
            }

            // Intersect angle
            if (closestIntersect.param == std::numeric_limits<float>::max()) continue;

            // Add to list of intersects
            intersects.push_back({closestIntersect.pos, angle});
        }
    }


    //static float timeElapsed = 0.f;
    //timeElapsed += dt;

    //mEmitterStepShader->Use();

    //mEmitterStepShader->SetUniform("DT", dt);
    //mEmitterStepShader->SetUniform("bufferMaxCount", MAX_BUFFER);
    ////1000 is the number of elements per grp
    //glDispatchCompute(MAX_BUFFER / WORK_GROUP, 1, 1);
    //glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    //mEmitterStepShader->Unuse();

    //mParticleShader->Use();
    //mParticleShader->SetUniform("DT", dt);
    ////mParticleShader->SetUniform("bufferMaxCount", MAX_BUFFER);
    ////1000 is the number of elements per grp
    //glDispatchCompute(MAX_BUFFER / WORK_GROUP, 1, 1);
    //glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    //mParticleShader->Unuse();

    //Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetFramebuffer(0)->Bind();
    //Draw();
    //Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetFramebuffer(0)->Unbind();
    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, mEmitterSSbo);
    //GLSLStructs::Emitter* vels = (GLSLStructs::Emitter*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_BUFFER * sizeof(GLSLStructs::Emitter), GL_MAP_READ_BIT);
    //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, mParticleStartSSbo);
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

/**
 * @brief Draws the particles.
 *
 * This function draws the particles.
 */
void LightingSystem::Draw() {
    auto inputSystem = Coordinator::GetInstance()->GetSystem<InputSystem>();
    //for testing, arbitrary
    glm::vec2 point = { inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second };
    for (auto const& p : intersects) {
        Renderer::DrawLine(glm::vec3(point, 0), glm::vec3(p.pos, 0), { 1,0,0,1 });
        Renderer::DrawCircle(glm::vec3(p.pos, 0), { 5, 5 }, {1,0,0,1});
    }
    //for (auto const& entity : mEntities) {
    //    auto const& collider = Coordinator::GetInstance()->GetComponent<Collider>(entity);
    //    // Assuming Vec2 is equivalent to glm::vec2
    //    glm::vec2 dimension = glm::vec2{ collider.dimension.x, collider.dimension.y }; // Example dimensions
    //    glm::vec2 position = glm::vec2{ collider.position.x, collider.position.y };  // Example position
    //    float rotation = collider.rotation; // Example rotation in radians

    //    // Start with an identity matrix
    //    glm::mat4 transformationMatrix = glm::mat4(1.0f);

    //    // Apply translation
    //    auto transMatrix = glm::translate(transformationMatrix, glm::vec3(position, 0.0f));

    //    // Apply rotation around the Z-axis
    //    auto rotMatrix = glm::rotate(transformationMatrix, rotation, glm::vec3(0.0f, 0.0f, 1.0f));

    //    // Apply scaling
    //    auto scaleMatrix = glm::scale(transformationMatrix, glm::vec3(dimension, 1.0f));

    //    transformationMatrix = transMatrix * rotMatrix * scaleMatrix;
    //    std::vector<glm::vec2> squarePoints = {
    //        glm::vec2(-0.5f, -0.5f),
    //        glm::vec2(-0.5f,  0.5f),
    //        glm::vec2(0.5f,   0.5f),
    //        glm::vec2(0.5f,  -0.5f)
    //    };

    //    for (auto& p : squarePoints) {
    //        // Convert glm::vec2 to glm::vec4, setting z to 0, and w to 1 for the transformation
    //        glm::vec4 pointVec4 = glm::vec4(p, 0.0f, 1.0f);

    //        // Apply the transformation
    //        glm::vec4 transformedVec4 = transformationMatrix * pointVec4;
    //        p = glm::vec2(transformedVec4);
    //        Renderer::DrawCircle(glm::vec3(p, 10), { 5, 5 }, { 1,0,0,1 });
    //        // Convert back to glm::vec2 and store it
    //        //points.push_back({ , std::atan2(transformedVec4.y - point.y, transformedVec4.x - point.x) });
    //    }
    //}
    ////glEnable(GL_DEPTH_TEST);
    ////glEnable(GL_CULL_FACE);
    ////glCullFace(GL_BACK);
    //mParticleRenderShader->Use();
    //auto& cam{ Coordinator::GetInstance()->GetComponent<Camera>(Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetCamera()) };

    //glm::mat4 viewprojection{ cam.GetViewProjMtx() };
    //mParticleRenderShader->SetUniform("vertViewProjection", viewprojection);
    //glDrawArrays(GL_POINTS, 0, MAX_BUFFER);
    //mParticleRenderShader->Unuse();

}
void LightingSystem::DrawDebug() {
    for (auto const& e : mEntities) {

    }
}

/**
 * @brief Destroys the particle system.
 *
 * This function destroys the particle system by deleting the buffers and shaders.
 */
void LightingSystem::Destroy() {
    //glDeleteBuffers(1, &mRandomIdxSSbo);
    //glDeleteBuffers(1, &mRandomSSbo);
    //glDeleteBuffers(1, &mEmitterSSbo);
    //glDeleteBuffers(1, &mParticleSSbo);
    //glDeleteBuffers(1, &mParticleCountSSbo);
    //glDeleteBuffers(1, &mParticleStartSSbo);
}