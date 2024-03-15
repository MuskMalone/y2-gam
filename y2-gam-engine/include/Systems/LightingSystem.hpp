/******************************************************************************/
/*!
\par        Image Engine
\file       LightingSystem.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		light

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once
#include <Core/Coordinator.hpp>
#include <Graphics/Shader.hpp>
#include <Core/Component.hpp>
#include <Graphics/VertexBuffer.hpp>
#include <Graphics/ElementBuffer.hpp>
#include <Graphics/VertexArray.hpp>

class LightBlockingSystem : public System {
public:
    std::set<Entity> const& GetLightBlockers() const { return mEntities; }
};

class LightingSystem : public System
{
    struct Point {
        glm::vec2 pos;
        float angle;
    };
    struct Intersect {
        glm::vec2 pos;
        float param;
    };
    struct Ray {
        glm::vec2 start;
        glm::vec2 end;
    };
    struct LightVertex {
        glm::vec2 origin;
        glm::vec3 color;
        glm::vec2 pos;
        float radius;
        float intensity;
    };
    /**
* @brief Calculates the intersection point between a ray and a line segment in 2D space.
*
* @param ray The ray defined by a start point and an end point.
* @param segment The line segment defined by a start point and an end point.
* @param result A reference to an Intersect object that will store the intersection point and parametric value (T1) if an intersection is found.
*
* @return true if an intersection is found, false otherwise.
*
* The function uses parametric equations to determine the intersection point between the ray and the line segment.
* It first checks if the ray and segment are parallel, in which case there is no intersection.
* It then solves for the parametric values T1 and T2 using the equations derived from the parametric equations of the ray and the segment.
* If an intersection is found and it lies within the valid range of the ray and the segment, the function calculates the intersection point
* using the parametric equation of the ray with the value of T1. It stores the intersection point and the value of T1 in the result object.
*/
    inline bool getIntersection(Ray ray, Ray segment, Intersect &result) {

        // RAY in parametric: Point + Delta*T1
        float r_px = ray.start.x;
        float r_py = ray.start.y;
        float r_dx = ray.end.x - ray.start.x;
        float r_dy = ray.end.y - ray.start.y;

        // SEGMENT in parametric: Point + Delta*T2
        float s_px = segment.start.x;
        float s_py = segment.start.y;
        float s_dx = segment.end.x - segment.start.x;
        float s_dy = segment.end.y - segment.start.y;

        // Are they parallel? If so, no intersect
        float r_mag = glm::sqrt(r_dx * r_dx + r_dy * r_dy);
        float s_mag = glm::sqrt(s_dx * s_dx + s_dy * s_dy);
        if (r_dx / r_mag == s_dx / s_mag && r_dy / r_mag == s_dy / s_mag) {
            // Unit vectors are the same.
            return false;
        }

        // SOLVE FOR T1 & T2
        // r_px+r_dx*T1 = s_px+s_dx*T2 && r_py+r_dy*T1 = s_py+s_dy*T2
        // ==> T1 = (s_px+s_dx*T2-r_px)/r_dx = (s_py+s_dy*T2-r_py)/r_dy
        // ==> s_px*r_dy + s_dx*T2*r_dy - r_px*r_dy = s_py*r_dx + s_dy*T2*r_dx - r_py*r_dx
        // ==> T2 = (r_dx*(s_py-r_py) + r_dy*(r_px-s_px))/(s_dx*r_dy - s_dy*r_dx)
        float T2 = (r_dx * (s_py - r_py) + r_dy * (r_px - s_px)) / (s_dx * r_dy - s_dy * r_dx);
        float T1 = (s_px + s_dx * T2 - r_px) / r_dx;

        // Must be within parametic whatevers for RAY/SEGMENT
        if (T1 < 0) return false;
        if (T2 < 0 || T2>1) return false;

        // Return the POINT OF INTERSECTION
        result = {
            {r_px + r_dx * T1,
            r_py + r_dy * T1,},
            T1
        };
        return true;
    }
    //std::deque<Point> intersects{};
    //std::vector<LightVertex>vertices{};
    std::vector<unsigned int> lightIndices{};
    //std::shared_ptr<Shader> mEmitterStepShader;
    //std::shared_ptr<Shader> mParticleShader;
    std::shared_ptr<Shader> mLightRenderPrePassShader;
    std::shared_ptr<Shader> mLightRenderShader;

    std::shared_ptr<VertexArray> mLightVertexArray;
    std::shared_ptr<VertexBuffer> mLightVertexBuffer;

    void EventListener(Event&);
public:
    void Init();
    void Update(unsigned int tex, unsigned int outtex);
    void Draw(unsigned int tex, unsigned int outtex);
    void DrawDebug();
    void Destroy();
};
