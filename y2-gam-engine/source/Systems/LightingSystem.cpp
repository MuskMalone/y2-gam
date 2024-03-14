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
#define MAX_VERTICES 27 // num of vertices per light blocking element
#define WORK_GROUP 100 //max buffer should be divisible by work group
//namespace { // for testing
//    float radius = 50.f;
//}
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
        {AttributeType::FLOAT, "radius"},
        {AttributeType::FLOAT, "intensity"}
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
void LightingSystem::Update(unsigned int tex, unsigned int outtex) {
    UNREFERENCED_PARAMETER(tex);
    UNREFERENCED_PARAMETER(outtex);
    Renderer::RenderFullscreenTexture(tex, mLightRenderPrePassShader);
    unsigned drawnlight{};
    for (auto const& lightEntity : mEntities) {
        auto const& light = Coordinator::GetInstance()->GetComponent<Light>(lightEntity);
        glm::vec2 point = light.pos;//{ inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second };
        float radius = light.radius;
        glm::vec3 clr = light.color;
        float intensity = light.intensity;

        auto& cam{ Coordinator::GetInstance()->GetComponent<Camera>(Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetCamera()) };
        glm::mat4 viewprojection{ cam.GetViewProjMtx() };

        glm::vec2 cmin = glm::vec2(viewprojection * glm::vec4(point.x - radius, point.y - radius, 0, 1));
        glm::vec2 cmax = glm::vec2(viewprojection * glm::vec4(point.x + radius, point.y + radius, 0, 1));
        glm::vec2 rmin{-1, -1};
        glm::vec2 rmax{1, 1};
        if (!(cmin.x < rmax.x && cmax.x > rmin.x && cmin.y < rmax.y && cmax.y > rmin.y)) continue;
        drawnlight++;
        std::deque<Point> intersects{};
        std::vector<LightVertex>vertices{};

        std::vector<Point> points;
        std::vector<Ray> segments{};
        //for testing, arbitrary
        auto const& lightblockers = Coordinator::GetInstance()->GetSystem<LightBlockingSystem>()->GetLightBlockers();
        for (auto const& entity : lightblockers) {

            auto const& lightblocker = Coordinator::GetInstance()->GetComponent<LightBlocker>(entity);
            // Assuming Vec2 is equivalent to glm::vec2
            glm::vec2 position = glm::vec2{ lightblocker.position.x, lightblocker.position.y };  // Example position

            glm::vec2 dimension = glm::vec2{ lightblocker.dimension.x, lightblocker.dimension.y }; // Example dimensions
            float rotation = lightblocker.rotation; // Example rotation in radians

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

        }
        if (points.empty()) {
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
        }

        for (auto const& p : points) {
            std::array<float, 3> angles{
                p.angle - 0.0001f, p.angle, p.angle + 0.0001f
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
                intersects.push_back({ closestIntersect.pos, angle });
            }
        }

        auto last = std::unique(intersects.begin(), intersects.end(), [](const Point& a, const Point& b) {
            return glm::distance(a.pos, b.pos) <= FLT_EPSILON;
            });
        intersects.erase(last, intersects.end());
        std::sort(intersects.begin(), intersects.end(), [](Point const& a, Point const& b) {
            return a.angle < b.angle;
            });

        intersects.push_front({ point, 0.f });
        for (int i{ 1 }; i < intersects.size(); ++i) {
            vertices.push_back({ intersects[0].pos, clr, intersects[0].pos, radius, intensity });
            vertices.push_back({ intersects[0].pos, clr, intersects[i].pos, radius, intensity });
            if (i + 1 < intersects.size()) {
                vertices.push_back({ intersects[0].pos, clr, intersects[i + 1].pos, radius, intensity });
            }
            else {
                vertices.push_back({ intersects[0].pos, clr, intersects[1].pos, radius, intensity });
            }
        }

        { // draw the array
            mLightVertexBuffer->SetData(vertices.data(), vertices.size() * sizeof(LightVertex));
            mLightRenderShader->Use();

            auto& cam{ Coordinator::GetInstance()->GetComponent<Camera>(Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetCamera()) };

            glm::mat4 viewprojection{ cam.GetViewProjMtx() };

            mLightRenderShader->SetUniform("screenTex", 0);
            mLightRenderShader->SetUniform("u_ViewProjMtx", viewprojection);

            //set the active texture unit to 0
            glActiveTexture(GL_TEXTURE0);

            //bind the texture you want to render
            glBindTexture(GL_TEXTURE_2D, outtex);


            mLightVertexArray->Bind();

            //draw the full-screen quad
            // glDrawElements(GL_TRIANGLES, (vertices.size() - 2) * MAX_VERTICES, GL_UNSIGNED_INT, nullptr);
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
            glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
            mLightVertexArray->Unbind();
            glBindTexture(GL_TEXTURE_2D, 0);
            mLightRenderShader->Unuse();
        }
    }
    //std::cout << "drawn light " << drawnlight << std::endl;
}

/**
 * @brief Draws the particles.
 *
 * This function draws the particles.
 */
void LightingSystem::Draw(unsigned int tex, unsigned int outtex) {
    //GLboolean scissorTestEnabled;
    //glGetBooleanv(GL_SCISSOR_TEST, &scissorTestEnabled);
    //glDisable(GL_SCISSOR_TEST);
    //GLboolean scissorTestEnabled = glIsEnabled(GL_SCISSOR_TEST);
    //if (scissorTestEnabled)
    //    glEnable(GL_SCISSOR_TEST);

    this->Update(tex, outtex);

    //glDisable(GL_SCISSOR_TEST);
    //if (scissorTestEnabled)
    //    glEnable(GL_SCISSOR_TEST);
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
    //auto const& lightblockers = Coordinator::GetInstance()->GetSystem<LightBlockingSystem>()->GetLightBlockers();
    //for (auto const& entity : lightblockers) {
    //    auto const& lightblocker = Coordinator::GetInstance()->GetComponent<LightBlocker>(entity);
    //    // Assuming Vec2 is equivalent to glm::vec2
    //    glm::vec2 position = glm::vec2{ lightblocker.position.x, lightblocker.position.y };  // Example position

    //    glm::vec2 dimension = glm::vec2{ lightblocker.dimension.x, lightblocker.dimension.y }; // Example dimensions
    //    float rotation = lightblocker.rotation; // Example rotation in radians

    //    Renderer::DrawQuad(glm::vec3{ position,1 }, dimension, glm::vec4{ 1.f, 0.f, 1.f ,1.f }, Image::Degree(rotation) );

    //}
}

/**
 * @brief Destroys the particle system.
 *
 * This function destroys the particle system by deleting the buffers and shaders.
 */
void LightingSystem::Destroy() {
}