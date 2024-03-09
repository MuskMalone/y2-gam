#pragma once
#include <Core/Coordinator.hpp>
#include <Graphics/Shader.hpp>
#include <Core/Component.hpp>


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
    std::vector<Point> intersects{};
    ////this is for adding emitters to the system
    std::shared_ptr<Shader> mLightShader;
    //std::shared_ptr<Shader> mEmitterStepShader;
    //std::shared_ptr<Shader> mParticleShader;
    std::shared_ptr<Shader> mLightRenderShader;

    GLuint mLightSSbo;
    GLuint mBlockSSbo;
    GLuint mVerticesSSbo;

    void EventListener(Event&);
public:
    void Init();
    void Update(float dt);
    void Draw();
    void DrawDebug();
    void Destroy();
};
