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
#include "glm/gtx/norm.hpp"
#include <glm/gtx/closest_point.hpp>
#include "Systems/LightingSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Engine/SceneManager.hpp"
#include "Core/Component.hpp"
#define MAX_BUFFER (19683)//3^9 // num of light sources/light blocking elements
#define MAX_VERTICES 3 // num of vertices per light blocking element
#define WORK_GROUP 100 //max buffer should be divisible by work group
namespace { // for testing
    float radius = 50.f;
}
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
    mLightVertexArray = VertexArray::Create();
    mLightVertexBuffer = VertexBuffer::Create(MAX_BUFFER * MAX_VERTICES * sizeof(LightVertex));
    BufferLayout lightVtxLayout{
        {AttributeType::VEC2, "origin"},
        {AttributeType::VEC3, "color"},
        {AttributeType::VEC2, "pos"},
        {AttributeType::FLOAT, "radius"}
    };
    mLightVertexBuffer->SetLayout(lightVtxLayout);
    mLightVertexArray->AddVertexBuffer(mLightVertexBuffer);

    mLightRenderShader = std::make_shared<Shader>("../assets/shaders/Light.vert", "../assets/shaders/Light.frag");
    mLightRenderPrePassShader = std::make_shared<Shader>("../assets/shaders/LightPrePass.vert", "../assets/shaders/LightPrePass.frag");

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
    vertices.clear();
    std::vector<Point> points;
    std::vector<Ray> segments{};
    auto inputSystem = Coordinator::GetInstance()->GetSystem<InputSystem>();
    glm::vec2 point = { inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second };
    
    auto& cam{ Coordinator::GetInstance()->GetComponent<Camera>(Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetCamera()) };

    glm::mat4 viewprojection{ cam.GetViewProjMtx() };
    glm::mat4 inviewprojection {glm::inverse(viewprojection) };
    std::array<glm::vec4, 4> corners{
        glm::vec4(-1.f, -1.f, 0, 1),
        glm::vec4(-1.f, 1.f, 0, 1),
        glm::vec4(1.f, 1.f, 0, 1),
        glm::vec4(1.f, -1.f, 0, 1)
    };

    for (auto& c : corners) {
        glm::vec4 transformedVec4 = inviewprojection * c;
        points.push_back({ glm::vec2(c = transformedVec4), std::atan2(transformedVec4.y - point.y, transformedVec4.x - point.x) });
    }
    for (int i{}; i < 4; ++i) {
        if (i != 3)
            segments.push_back(Ray{ corners[i], corners[i + 1] });
        else
            segments.push_back(Ray{ corners[3], corners[0] });
    }
    //for testing, arbitrary
    for (auto const& entity : mEntities) {
       
        auto const& collider = Coordinator::GetInstance()->GetComponent<Collider>(entity);
        // Assuming Vec2 is equivalent to glm::vec2
        glm::vec2 position = glm::vec2{ collider.position.x, collider.position.y };  // Example position
        
            glm::vec2 dimension = glm::vec2{ collider.dimension.x, collider.dimension.y }; // Example dimensions
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
                points.push_back({ glm::vec2(p = transformedVec4), std::atan2(transformedVec4.y - point.y, transformedVec4.x - point.x) });
            }
            for (int i{}; i < 4; ++i) {
                if (i != 3)
                    segments.push_back(Ray{ squarePoints[i], squarePoints[i + 1] });
                else
                    segments.push_back(Ray{ squarePoints[3], squarePoints[0] });
            }
            //    points.push_back({ p , std::atan2(start.y - point.y, start.x - point.x) });

            //}
            //for (int i{}; i < 4; ++i) {
            //    glm::vec2 start;
            //    glm::vec2 end;
            //    if (i != 3) {
            //        start = squarePoints[i], end = squarePoints[i + 1];
            //    }
            //    else {
            //        start = squarePoints[3], end = squarePoints[0];
            //    }
            //    if (glm::distance2(glm::closestPointOnLine(point, start, end), point) <= radius * radius) {
            //        points.push_back({ end , std::atan2(start.y - point.y, start.x - point.x) });
            //        segments.push_back({ start, end });
            //    }

            //}
        
    }
    for (auto const& p : points) {
        std::array<float, 3> angles{
            p.angle - 0.00001f, p.angle, p.angle + 0.00001f
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
void LightingSystem::Draw(unsigned int tex) {
    Renderer::RenderFullscreenTexture(tex, mLightRenderPrePassShader);

    auto inputSystem = Coordinator::GetInstance()->GetSystem<InputSystem>();
    //for testing, arbitrary
    glm::vec2 point = { inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second };

    auto last = std::unique(intersects.begin(), intersects.end(), [](const Point& a, const Point& b) {
        return glm::distance(a.pos, b.pos) <= FLT_EPSILON;
        });
    intersects.erase(last, intersects.end());
    std::sort(intersects.begin(), intersects.end(), [](Point const& a, Point const& b) {
        return a.angle < b.angle;
        });
    auto& cam{ Coordinator::GetInstance()->GetComponent<Camera>(Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetCamera()) };

    glm::mat4 viewprojection{ cam.GetViewProjMtx() };

    intersects.push_front({ point, 0.f });
    for (int i{ 1 }; i < intersects.size(); ++i) {
        vertices.push_back({ intersects[0].pos, { 0,1,1 }, intersects[0].pos, radius });
        vertices.push_back({ intersects[0].pos,{0,1,1}, intersects[i].pos, radius});
        if (i + 1 < intersects.size()) {
            vertices.push_back({ intersects[0].pos,{0,1,1}, intersects[i+1].pos, radius });
        }
        else {
            vertices.push_back({ intersects[0].pos,{0,1,1}, intersects[1].pos, radius });
        }
    }
    //vertices.push_back({{ 1,1,1 }, { -50, -50 }, radius});
    //vertices.push_back({ { 1,1,1 }, { -50, 50 }, radius });
    //vertices.push_back({ { 1,1,1 }, { 50, 50 }, radius });

    //std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    //for (int i{}; i < vertices.size(); i += 3) {
    //    std::cout << "(" << vertices[i].pos.x << "," << vertices[i].pos.y << "), ";
    //    std::cout << "(" << vertices[i+1].pos.x << "," << vertices[i+1].pos.y << "), ";
    //    std::cout << "(" << vertices[i+2].pos.x << "," << vertices[i+2].pos.y << ")\n";
    //}
    //std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";

    mLightVertexBuffer->SetData(vertices.data(), vertices.size() * sizeof(LightVertex));
    mLightRenderShader->Use();


    mLightRenderShader->SetUniform("screenTex", 0);
    mLightRenderShader->SetUniform("u_ViewProjMtx", viewprojection);

    //set the active texture unit to 0
    glActiveTexture(GL_TEXTURE0);

    //bind the texture you want to render
    glBindTexture(GL_TEXTURE_2D, tex);


    mLightVertexArray->Bind();

    //draw the full-screen quad
    // glDrawElements(GL_TRIANGLES, (vertices.size() - 2) * MAX_VERTICES, GL_UNSIGNED_INT, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    mLightVertexArray->Unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
    mLightRenderShader->Unuse();
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
    //auto inputSystem = Coordinator::GetInstance()->GetSystem<InputSystem>();
    ////for testing, arbitrary
    //glm::vec2 point = { inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second };

    //auto last = std::unique(intersects.begin(), intersects.end(), [](const Point& a, const Point& b) {
    //    return glm::distance(a.pos, b.pos) <= FLT_EPSILON;
    //    });
    //intersects.erase(last, intersects.end());
    //std::sort(intersects.begin(), intersects.end(), [](Point const& a, Point const& b) {
    //    return a.angle < b.angle;
    //    });

    //for (int i{}; i < intersects.size(); ++i) {
    //    auto const& p{ intersects[i] };
    //    Renderer::DrawLine(glm::vec3(point, 0), glm::vec3(p.pos, 0), { 1,(float)i / (float)intersects.size(),0,1 });
    //    Renderer::DrawCircle(glm::vec3(p.pos, 0), { 1, 1 }, { 1,(float)i / (float)intersects.size(),0,1 });
    //}
}

/**
 * @brief Destroys the particle system.
 *
 * This function destroys the particle system by deleting the buffers and shaders.
 */
void LightingSystem::Destroy() {
}