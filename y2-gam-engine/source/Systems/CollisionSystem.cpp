#include "Systems/CollisionSystem.hpp"
#include <memory>
#include "Core/Coordinator.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/RigidBody.hpp"
#include <Core/Globals.hpp>
#include <Math/Collision.hpp>
#include <glm/common.hpp>
#include <chrono>
#include <Core/Types.hpp>

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}
namespace Collision{

    inline constexpr Vec2 vabs(Vec2 const& v) { return Vec2{ fabs(v.x), fabs(v.y) }; }
    inline constexpr Mat22 mabs(Mat22 const&m) { return Mat22{vabs(m[0]),vabs(m[1])}; }
    Mat22 Mat22FromAngle(float rads) {
        float c = cosf(rads);
        float s = sinf(rads);
        return Mat22{
            c, s,
            -s, c
        };
    }
    Mat33 Mat33FromAngle(float rads) {
        float c = cosf(rads);
        float s = sinf(rads);
        return Mat33{
            c, s, 0,
            -s, c, 0,
            0, 0, 1
        };
    }
    Mat33 Mat33FromTranslate(float x, float y) {
        return Mat33{
            1,0,0,
            0,1,0,
            x,y,1
        };
    }
    using namespace Physics;
    //returns the min and max values of the 
    std::pair<Vec2, Vec2> GetAABBBody(RigidBody const& rb) {
        auto transform{ Mat33FromTranslate(rb.position.x, rb.position.y) * Mat33FromAngle(rb.rotation) };
        Vec2 topleft{ transform * Vec3{ rb.dimension.x * -.5f, rb.dimension.y * .5f, 1.f } };
        Vec2 topright{ transform * Vec3{ rb.dimension.x * .5f, rb.dimension.y * .5f, 1.f } };
        Vec2 bottomleft{ transform * Vec3{ rb.dimension.x * -.5f, rb.dimension.y * -.5f, 1.f } };
        Vec2 bottomright{ transform * Vec3{ rb.dimension.x * .5f, rb.dimension.y * -.5f, 1.f } };
        return {
            Vec2{std::min({topleft.x, topright.x, bottomleft.x, bottomright.x}), std::min({topleft.y, topright.y, bottomleft.y, bottomright.y})},
            Vec2{std::max({topleft.x, topright.x, bottomleft.x, bottomright.x}), std::max({topleft.y, topright.y, bottomleft.y, bottomright.y})}
        };
    }

    int ClipSegmentToLine(ClipVertex vOut[2], ClipVertex vIn[2], const Vec2& normal, float offset,
        float clipEdge) {
        // Start with no output points
        int numOut = 0;

        // Calculate the distance of end points to the line
        float distance0 = glm::dot(normal, vIn[0].v) - offset;
        float distance1 = glm::dot(normal, vIn[1].v) - offset;

        // If the points are behind the plane
        if (distance0 <= 0.0f) {
            vOut[numOut] = vIn[0];
            numOut++;
        }
        if (distance1 <= 0.0f) {
            vOut[numOut] = vIn[1];
            numOut++;
        }

        // If the points are on different sides of the plane
        if (distance0 * distance1 < 0.0f) {
            // Find intersection point of edge and plane
            float interp = distance0 / (distance0 - distance1);
            vOut[numOut].v = vIn[0].v + (vIn[1].v - vIn[0].v) * interp;
            if (distance0 > 0.0f) {
                vOut[numOut].fp = vIn[0].fp;
                vOut[numOut].fp.e.inEdge1 = static_cast<EdgeNumbers>(clipEdge);
                vOut[numOut].fp.e.inEdge2 = EdgeNumbers::NO_EDGE;
            }
            else {
                vOut[numOut].fp = vIn[1].fp;
                vOut[numOut].fp.e.outEdge1 = static_cast<EdgeNumbers>(clipEdge);
                vOut[numOut].fp.e.outEdge2 = EdgeNumbers::NO_EDGE;
            }

            numOut++;
        }

        return numOut;
    }

    void ComputeIncidentEdge(ClipVertex c[2], const Vec2& h, const Vec2& pos, const Mat22& rot,
        const Vec2& normal) {
        // The normal is from the reference box. Convert it
        // to the incident boxe's frame and flip sign.

        //inverse the rotation
        Mat22 rotT = glm::transpose(rot);//Matrix2x2Transpose(rot);
        Vec2 n = (rotT * normal) * (-1.0f);
        Vec2 nAbs{ vabs(n) };

        if (nAbs.x > nAbs.y) {
            if (n.x >= 0.0f) {
                c[0].v = { h.x, -h.y };
                c[0].fp.e.inEdge2 = EdgeNumbers::EDGE3;
                c[0].fp.e.outEdge2 = EdgeNumbers::EDGE4;

                c[1].v = { h.x, h.y };
                c[1].fp.e.inEdge2 = EdgeNumbers::EDGE4;
                c[1].fp.e.outEdge2 = EdgeNumbers::EDGE1;
            }
            else {
                c[0].v = { -h.x, h.y };
                c[0].fp.e.inEdge2 = EdgeNumbers::EDGE1;
                c[0].fp.e.outEdge2 = EdgeNumbers::EDGE2;

                c[1].v = { -h.x, -h.y };
                c[1].fp.e.inEdge2 = EdgeNumbers::EDGE2;
                c[1].fp.e.outEdge2 = EdgeNumbers::EDGE3;
            }
        }
        else {
            if (n.y >= 0.0f) {
                c[0].v = { h.x, h.y };
                c[0].fp.e.inEdge2 = EdgeNumbers::EDGE4;
                c[0].fp.e.outEdge2 = EdgeNumbers::EDGE1;

                c[1].v = { -h.x, h.y };
                c[1].fp.e.inEdge2 = EdgeNumbers::EDGE1;
                c[1].fp.e.outEdge2 = EdgeNumbers::EDGE2;
            }
            else {
                c[0].v = { -h.x, -h.y };
                c[0].fp.e.inEdge2 = EdgeNumbers::EDGE2;
                c[0].fp.e.outEdge2 = EdgeNumbers::EDGE3;

                c[1].v = { h.x, -h.y };
                c[1].fp.e.inEdge2 = EdgeNumbers::EDGE3;
                c[1].fp.e.outEdge2 = EdgeNumbers::EDGE4;
            }
        }

        c[0].v = pos + rot * c[0].v;
        c[1].v = pos + rot * c[1].v;
    }

    uint32_t Collide(Physics::Contact* contacts, RigidBody& b1, RigidBody& b2) {


        Vec2 h1 = b1.dimension * 0.5f;
        Vec2 h2 = b2.dimension * 0.5f;

        Vec2 pos1 = b1.position;
        Vec2 pos2 = b2.position;

        Mat22 rot1 = Mat22FromAngle(b1.rotation);
        Mat22 rot2 = Mat22FromAngle(b2.rotation);

        Mat22 rot1T = glm::transpose(rot1); //inverse the rotation
        Mat22 rot2T = glm::transpose(rot2);

        Vec2 dp = pos2 - pos1;
        Vec2 d1 = rot1T * dp;
        Vec2 d2 = rot2T * dp;

        Mat22 C = rot1T * rot2;
        Mat22 absC = mabs(C);//Matrix2x2Abs(C);
        Mat22 absCT = glm::transpose(absC);

        // Box 1 faces
        Vec2 face1 = vabs (d1)-h1 - (absC * h2);
        if (face1.x > 0.0f || face1.y > 0.0f) {
            return 0;
        }

        // Box 2 faces
        Vec2 face2 = vabs(d2) - h2 - (absCT * h1);
        if (face2.x > 0.0f || face2.y > 0.0f) {
            return 0;
        }

        // Find best axis
        Axis axis;
        float seperation;
        Vec2 normal;
            // Box 1 faces
        axis = Axis::FACE_A_X;
        seperation = face1.x;
        normal = (d1.x > 0.0f) ? rot1[0] : rot1[0] * (-1.0f);

        const float relativeToL = 0.95f;
        const float absoluteToL = 0.01f;

        if (face1.y > relativeToL * seperation + absoluteToL * h1.y) {
            axis = Axis::FACE_A_Y;
            seperation = face1.y;
            normal = (d1.y > 0.0f) ? rot1[1] : rot1[1] * (-1.0f);
        }
            
        // Box 2 faces
        if (face2.x > relativeToL * seperation + absoluteToL * h2.x) {
            axis = Axis::FACE_B_X;
            seperation = face2.x;
            normal = (d2.x > 0.0f) ? rot2[0] : rot2[0] * (-1.0f);
        }

        if (face2.y > relativeToL * seperation + absoluteToL * h2.y) {
            axis = Axis::FACE_B_Y;
            seperation = face2.y;
            normal = (d2.y > 0.0f) ? rot2[1] : rot2[1] * (-1.0f);
        }

        // Setup clipping plane data based on the separating axis
        Vec2 frontNormal, sideNormal;
        ClipVertex incidentEdge[2];
        float front, negSide, posSide;
        EdgeNumbers negEdge, posEdge;

        // Compute the clipping lines and the line segment to be clipped
        switch (axis) {
        case Axis::FACE_A_X: {
            frontNormal = normal;
            front = glm::dot(pos1, frontNormal) + h1.x;
            sideNormal = rot1[1];
            float side = glm::dot(pos1, sideNormal);
            negSide = -side + h1.y;
            posSide = side + h1.y;
            negEdge = EdgeNumbers::EDGE3;
            posEdge = EdgeNumbers::EDGE1;
            ComputeIncidentEdge(incidentEdge, h2, pos2, rot2, frontNormal);
        } break;
        case Axis::FACE_A_Y: {
            frontNormal = normal;
            front = glm::dot(pos1, frontNormal) + h1.y;
            sideNormal = rot1[0];
            float side = glm::dot(pos1, sideNormal);
            negSide = -side + h1.x;
            posSide = side + h1.x;
            negEdge = EdgeNumbers::EDGE2;
            posEdge = EdgeNumbers::EDGE4;
            ComputeIncidentEdge(incidentEdge, h2, pos2, rot2, frontNormal);
        } break;
        case Axis::FACE_B_X: {
            frontNormal = normal * (-1.0f);
            front = glm::dot(pos2, frontNormal) + h2.x;
            sideNormal = rot2[1];
            float side = glm::dot(pos2, sideNormal);
            negSide = -side + h2.y;
            posSide = side + h2.y;
            negEdge = EdgeNumbers::EDGE3;
            posEdge = EdgeNumbers::EDGE1;
            ComputeIncidentEdge(incidentEdge, h1, pos1, rot1, frontNormal);
        } break;
        case Axis::FACE_B_Y: {
            frontNormal = normal * (-1.0f);
            front = glm::dot(pos2, frontNormal) + h2.y;
            sideNormal = rot2[0];
            float side = glm::dot(pos2, sideNormal);
            negSide = -side + h2.x;
            posSide = side + h2.x;
            negEdge = EdgeNumbers::EDGE2;
            posEdge = EdgeNumbers::EDGE4;
            ComputeIncidentEdge(incidentEdge, h1, pos1, rot1, frontNormal);
        } break;
        }

        // clip other face with 5 box planes (1 face plane, 4 edge planes)
        ClipVertex clipPoints1[2];
        ClipVertex clipPoints2[2];
        int np;

        // Clip to box side 1
        np = ClipSegmentToLine(clipPoints1, incidentEdge, sideNormal * (-1.0f), negSide, static_cast<float>(static_cast<uint32_t>(negEdge)));

        if (np < 2) {
            return 0;
        }

        // Clip to negative box side 1
        np = ClipSegmentToLine(clipPoints2, clipPoints1, sideNormal, posSide, static_cast<float>(static_cast<uint32_t>(posEdge)));

        if (np < 2) {
            return 0;
        }

        // Now clipPoints2 contains the clipping points.
        // Due to roundoff, it is possible that clipping removes all points

        uint32_t numContacts = 0;
        for (uint32_t i = 0; i < 2; i++) {
            float seperation = glm::dot(frontNormal, clipPoints2[i].v) - front;

            if (seperation <= 0) {
                contacts[numContacts].seperation = seperation;
                contacts[numContacts].normal = normal;
                // slide contact point onto reference face (easy to cull)
                contacts[numContacts].position = clipPoints2[i].v - frontNormal * seperation;
                contacts[numContacts].feature = clipPoints2[i].fp;

                if (axis == Axis::FACE_B_X || axis == Axis::FACE_B_Y) {
                    std::swap(contacts[numContacts].feature.e.inEdge1,
                        contacts[numContacts].feature.e.inEdge2);
                    std::swap(contacts[numContacts].feature.e.outEdge1,
                        contacts[numContacts].feature.e.outEdge2);
                }

                numContacts++;
            }
        }

        if (!b1.isGrounded) {
            b1.isGrounded = (normal.y < 0.0f);
        }

        return numContacts;
    }

    Arbiter Collide(Entity b1, Entity b2) {
        auto & rb1{ Coordinator::GetInstance()->GetComponent<RigidBody>(b1) };
        auto & rb2{ Coordinator::GetInstance()->GetComponent<RigidBody>(b2) };

        auto aabb1{ GetAABBBody(rb1) };
        auto aabb2{ GetAABBBody(rb2) };
        //////trivial reject
        if (!CheckAABBDiscrete(aabb1.first, aabb1.second, aabb2.first, aabb2.second)) {
            //std::cout << "reject\n";
            return Arbiter{};
        }

        Arbiter result {};

        result.b1 = b1;
        result.b2 = b2;

        result.combinedFriction = sqrtf(rb1.friction * rb2.friction);
        result.contactsCount = Collide(result.contacts, rb1, rb2);

        return result;
    }

    void CollisionSystem::Init() {
        gCoordinator = Coordinator::GetInstance();
        using namespace DataMgmt;
        mQuadtree = DataMgmt::Quadtree<Entity>{ 0, Rect(Vec2(-WORLD_LIMIT_X, -WORLD_LIMIT_Y), Vec2(WORLD_LIMIT_X, WORLD_LIMIT_Y)) };
    }

    void CollisionSystem::Update(float dt) {
        //for (auto const& entity : mEntities) {
        //	auto& collider = gCoordinator->GetComponent<BoxCollider>(entity);
        //	auto& rigidBody = gCoordinator->GetComponent<RigidBody>(entity);


        //}

        mQuadtree.Update(mEntities, [](Entity const& e, DataMgmt::Rect const& r) {
            auto const& rigidBody{ Coordinator::GetInstance()->GetComponent<RigidBody>(e) };
            
            //todo update the position based on rotated box not aabb
            //todo substep checking
            auto aabb = GetAABBBody(rigidBody);
            Vec2 rmin = r.GetMin();
            Vec2 rmax = r.GetMax();
            //basic aabb check
            return CheckAABBDiscrete(aabb.first, aabb.second, rmin, rmax);
        });
        std::vector<std::vector<Entity>> entity2dVec;
        mQuadtree.Get(entity2dVec);
        //std::cout << entity2dVec.size() << std::endl;
        //calculate each list of entities in each quad
        for (auto const& entityVec : entity2dVec) {
            for (int i{}; i < entityVec.size(); ++i) {
                for (int j{i + 1}; j < entityVec.size(); ++j) {
                    //if (entityVec[i] == entityVec[j]) continue;
                    Arbiter arbiter = Collide(entityVec[i], entityVec[j]);
                    ArbiterKey arbiterKey{ entityVec[i], entityVec[j] };
                    uint64_t hashTableKey = murmur64((void*)&arbiterKey, sizeof(ArbiterKey));

                    if (arbiter.contactsCount > 0) {
                        Event event{ Events::Physics::COLLISION };
                        event.SetParam(Events::Physics::Collision::COLLIDED, ArbiterPair {hashTableKey, arbiter});
                        gCoordinator->SendEvent(event);

                    }


                }
            }
        }
    }

    void CollisionSystem::Debug() {
        mQuadtree.Debug();
        //auto& camera = Coordinator::GetInstance()->GetComponent<OrthoCamera>(Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetCamera());
        //Renderer::RenderSceneBegin(camera);
        size_t sizeent{ mEntities.size() };

        for (auto const& e : mEntities) {
            auto const& rb{ Coordinator::GetInstance()->GetComponent<RigidBody>(e) };

            auto aabb{ GetAABBBody(rb) };
            auto scale{ aabb.second - aabb.first };
            Renderer::DrawLineRect(Vec3(aabb.first + scale / 2.f, 1), scale, { 1.f, 1.f, 1.f ,1.f });

        }
        //Renderer::RenderSceneEnd();

    }
}
