#include "Systems/CollisionSystem.hpp"
#include <memory>
#include "Core/Coordinator.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/RigidBody.hpp"
#include <Core/Globals.hpp>
#include <Math/Collision.hpp>
#include <glm/common.hpp>
#include <Systems/PhysicsSystem.hpp>
#include <chrono>

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
    void ArbiterMergeContacts(Arbiter& a, Arbiter toMerge) {
        Contact merged_contacts[2];

        for (uint32_t i = 0; i < toMerge.contacts_count; i++) {
            Contact& c_new{ toMerge.contacts[i] };
            int k = -1;
            for (uint32_t j = 0; j < a.contacts_count; j++) {
                Contact& c_old = *(a.contacts + j);
                if (c_new.feature.value == c_old.feature.value) {
                    k = j;
                    break;
                }
            }

            if (k > -1) {
                Contact& c{ merged_contacts[i] };
                Contact& c_old{ a.contacts[k] };
                c = c_new;

                // Warm starting
                c.acc_normal_impulse = c_old.acc_normal_impulse;
                c.acc_tangent_impulse = c_old.acc_tangent_impulse;
                c.acc_biased_normal_impulse = c_old.acc_biased_normal_impulse;
            }
            else {
                merged_contacts[i] = toMerge.contacts[i];
            }
        }

        for (uint32_t i = 0; i < toMerge.contacts_count; i++) {
            a.contacts[i] = merged_contacts[i];
        }

        a.contacts_count = toMerge.contacts_count;
    }
    int ClipSegmentToLine(ClipVertex v_out[2], ClipVertex v_in[2], const Vec2& normal, float offset,
        float clip_edge) {
        // Start with no output points
        int num_out = 0;

        // Calculate the distance of end points to the line
        float distance0 = glm::dot(normal, v_in[0].v) - offset;
        float distance1 = glm::dot(normal, v_in[1].v) - offset;

        // If the points are behind the plane
        if (distance0 <= 0.0f) {
            v_out[num_out] = v_in[0];
            num_out++;
        }
        if (distance1 <= 0.0f) {
            v_out[num_out] = v_in[1];
            num_out++;
        }

        // If the points are on different sides of the plane
        if (distance0 * distance1 < 0.0f) {
            // Find intersection point of edge and plane
            float interp = distance0 / (distance0 - distance1);
            v_out[num_out].v = v_in[0].v + (v_in[1].v - v_in[0].v) * interp;
            if (distance0 > 0.0f) {
                v_out[num_out].fp = v_in[0].fp;
                v_out[num_out].fp.e.in_edge_1 = static_cast<EdgeNumbers>(clip_edge);
                v_out[num_out].fp.e.in_edge_2 = EdgeNumbers::NO_EDGE;
            }
            else {
                v_out[num_out].fp = v_in[1].fp;
                v_out[num_out].fp.e.out_edge_1 = static_cast<EdgeNumbers>(clip_edge);
                v_out[num_out].fp.e.out_edge_2 = EdgeNumbers::NO_EDGE;
            }

            num_out++;
        }

        return num_out;
    }

    void ComputeIncidentEdge(ClipVertex c[2], const Vec2& h, const Vec2& pos, const Mat22& rot,
        const Vec2& normal) {
        // The normal is from the reference box. Convert it
        // to the incident boxe's frame and flip sign.

        //inverse the rotation
        Mat22 rotT = glm::transpose(rot);//Matrix2x2Transpose(rot);
        Vec2 n = (rotT * normal) * (-1.0f);
        Vec2 n_abs{ vabs(n) };

        if (n_abs.x > n_abs.y) {
            if (n.x >= 0.0f) {
                c[0].v = { h.x, -h.y };
                c[0].fp.e.in_edge_2 = EdgeNumbers::EDGE3;
                c[0].fp.e.out_edge_2 = EdgeNumbers::EDGE4;

                c[1].v = { h.x, h.y };
                c[1].fp.e.in_edge_2 = EdgeNumbers::EDGE4;
                c[1].fp.e.out_edge_2 = EdgeNumbers::EDGE1;
            }
            else {
                c[0].v = { -h.x, h.y };
                c[0].fp.e.in_edge_2 = EdgeNumbers::EDGE1;
                c[0].fp.e.out_edge_2 = EdgeNumbers::EDGE2;

                c[1].v = { -h.x, -h.y };
                c[1].fp.e.in_edge_2 = EdgeNumbers::EDGE2;
                c[1].fp.e.out_edge_2 = EdgeNumbers::EDGE3;
            }
        }
        else {
            if (n.y >= 0.0f) {
                c[0].v = { h.x, h.y };
                c[0].fp.e.in_edge_2 = EdgeNumbers::EDGE4;
                c[0].fp.e.out_edge_2 = EdgeNumbers::EDGE1;

                c[1].v = { -h.x, h.y };
                c[1].fp.e.in_edge_2 = EdgeNumbers::EDGE1;
                c[1].fp.e.out_edge_2 = EdgeNumbers::EDGE2;
            }
            else {
                c[0].v = { -h.x, -h.y };
                c[0].fp.e.in_edge_2 = EdgeNumbers::EDGE2;
                c[0].fp.e.out_edge_2 = EdgeNumbers::EDGE3;

                c[1].v = { h.x, -h.y };
                c[1].fp.e.in_edge_2 = EdgeNumbers::EDGE3;
                c[1].fp.e.out_edge_2 = EdgeNumbers::EDGE4;
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

        const float relative_to_l = 0.95f;
        const float absolute_to_l = 0.01f;

        if (face1.y > relative_to_l * seperation + absolute_to_l * h1.y) {
            axis = Axis::FACE_A_Y;
            seperation = face1.y;
            normal = (d1.y > 0.0f) ? rot1[1] : rot1[1] * (-1.0f);
        }
            
        // Box 2 faces
        if (face2.x > relative_to_l * seperation + absolute_to_l * h2.x) {
            axis = Axis::FACE_B_X;
            seperation = face2.x;
            normal = (d2.x > 0.0f) ? rot2[0] : rot2[0] * (-1.0f);
        }

        if (face2.y > relative_to_l * seperation + absolute_to_l * h2.y) {
            axis = Axis::FACE_B_Y;
            seperation = face2.y;
            normal = (d2.y > 0.0f) ? rot2[1] : rot2[1] * (-1.0f);
        }

        // Setup clipping plane data based on the separating axis
        Vec2 front_normal, side_normal;
        ClipVertex incident_edge[2];
        float front, neg_side, pos_side;
        EdgeNumbers neg_edge, pos_edge;

        // Compute the clipping lines and the line segment to be clipped
        switch (axis) {
        case Axis::FACE_A_X: {
            front_normal = normal;
            front = glm::dot(pos1, front_normal) + h1.x;
            side_normal = rot1[1];
            float side = glm::dot(pos1, side_normal);
            neg_side = -side + h1.y;
            pos_side = side + h1.y;
            neg_edge = EdgeNumbers::EDGE3;
            pos_edge = EdgeNumbers::EDGE1;
            ComputeIncidentEdge(incident_edge, h2, pos2, rot2, front_normal);
        } break;
        case Axis::FACE_A_Y: {
            front_normal = normal;
            front = glm::dot(pos1, front_normal) + h1.y;
            side_normal = rot1[0];
            float side = glm::dot(pos1, side_normal);
            neg_side = -side + h1.x;
            pos_side = side + h1.x;
            neg_edge = EdgeNumbers::EDGE2;
            pos_edge = EdgeNumbers::EDGE4;
            ComputeIncidentEdge(incident_edge, h2, pos2, rot2, front_normal);
        } break;
        case Axis::FACE_B_X: {
            front_normal = normal * (-1.0f);
            front = glm::dot(pos2, front_normal) + h2.x;
            side_normal = rot2[1];
            float side = glm::dot(pos2, side_normal);
            neg_side = -side + h2.y;
            pos_side = side + h2.y;
            neg_edge = EdgeNumbers::EDGE3;
            pos_edge = EdgeNumbers::EDGE1;
            ComputeIncidentEdge(incident_edge, h1, pos1, rot1, front_normal);
        } break;
        case Axis::FACE_B_Y: {
            front_normal = normal * (-1.0f);
            front = glm::dot(pos2, front_normal) + h2.y;
            side_normal = rot2[0];
            float side = glm::dot(pos2, side_normal);
            neg_side = -side + h2.x;
            pos_side = side + h2.x;
            neg_edge = EdgeNumbers::EDGE2;
            pos_edge = EdgeNumbers::EDGE4;
            ComputeIncidentEdge(incident_edge, h1, pos1, rot1, front_normal);
        } break;
        }

        // clip other face with 5 box planes (1 face plane, 4 edge planes)
        ClipVertex clip_points1[2];
        ClipVertex clip_points2[2];
        int np;

        // Clip to box side 1
        np = ClipSegmentToLine(clip_points1, incident_edge, side_normal * (-1.0f), neg_side, static_cast<float>(static_cast<uint32_t>(neg_edge)));

        if (np < 2) {
            return 0;
        }

        // Clip to negative box side 1
        np = ClipSegmentToLine(clip_points2, clip_points1, side_normal, pos_side, static_cast<float>(static_cast<uint32_t>(pos_edge)));

        if (np < 2) {
            return 0;
        }

        // Now clip_points2 contains the clipping points.
        // Due to roundoff, it is possible that clipping removes all points

        uint32_t num_contacts = 0;
        for (uint32_t i = 0; i < 2; i++) {
            float seperation = glm::dot(front_normal, clip_points2[i].v) - front;

            if (seperation <= 0) {
                contacts[num_contacts].seperation = seperation;
                contacts[num_contacts].normal = normal;
                // slide contact point onto reference face (easy to cull)
                contacts[num_contacts].position = clip_points2[i].v - front_normal * seperation;
                contacts[num_contacts].feature = clip_points2[i].fp;

                if (axis == Axis::FACE_B_X || axis == Axis::FACE_B_Y) {
                    std::swap(contacts[num_contacts].feature.e.in_edge_1,
                        contacts[num_contacts].feature.e.in_edge_2);
                    std::swap(contacts[num_contacts].feature.e.out_edge_1,
                        contacts[num_contacts].feature.e.out_edge_2);
                }

                num_contacts++;
            }
        }

        if (!b1.isGrounded) {
            b1.isGrounded = (normal.y < 0.0f);
        }

        return num_contacts;
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

        result.combined_friction = sqrtf(rb1.friction * rb2.friction);
        result.contacts_count = Collide(result.contacts, rb1, rb2);

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
            Vec2 rmin = r.get_min();
            Vec2 rmax = r.get_max();
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
                    //i have no clue why murmur works but chrono doesnt
                    uint64_t hashTableKey = murmur64((void*)&arbiterKey, sizeof(ArbiterKey));
                        //std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                    ArbiterHashTable& arbiterTable{ Coordinator::GetInstance()->GetSystem<PhysicsSystem>()->mArbiterTable };
                    ArbiterHashTable::iterator iter = arbiterTable.find(hashTableKey);
                    //HashTableGet(&world->arbiter_table, hashTableKey);

                    if (arbiter.contacts_count > 0) {
                        if (iter == arbiterTable.end()) {
                            //HashTableSet(&world->arbiter_table, hashTableKey, arbiter);
                           arbiterTable.emplace(hashTableKey, arbiter);
                        }
                        else {
                            ArbiterMergeContacts(iter->second, arbiter);
                        }
                    }


                }
            }
        }
    }

    void CollisionSystem::Debug() {
        mQuadtree.Debug();
        OrthoCamera cam{ -WORLD_LIMIT_X, WORLD_LIMIT_X, -WORLD_LIMIT_Y, WORLD_LIMIT_Y };
        Renderer::RenderSceneBegin(cam);
        size_t sizeent{ mEntities.size() };

        for (auto const& e : mEntities) {
            auto const& rb{ Coordinator::GetInstance()->GetComponent<RigidBody>(e) };

            auto aabb{ GetAABBBody(rb) };
            auto scale{ aabb.second - aabb.first };
            Renderer::DrawLineRect(Vec3(aabb.first + scale / 2.f, 1), scale, { 1.f, 1.f, 1.f ,1.f });

        }
        Renderer::RenderSceneEnd();

    }
}
