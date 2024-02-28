/******************************************************************************/
/*!
\par        Image Engine
\file       CollisionSystem.cpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		collision detection system

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"

#include "Systems/CollisionSystem.hpp"
#include "Core/Coordinator.hpp"
#include "Components/Collider.hpp"
#include "Systems/LayeringSystem.hpp"
//#include "Components/RigidBody.hpp"
#include <Core/Globals.hpp>
#include <Math/Collision.hpp>
#include <Core/Types.hpp>

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}
namespace Testing {
    Vec2 testingStart{
     40,-96.5
    }, testingEnd{
       40,-105.5
    };
}
namespace Collision{

    inline Vec2 vabs(Vec2 const& v) { return Vec2{ fabs(v.x), fabs(v.y) }; }
    inline Mat22 mabs(Mat22 const&m) { return Mat22{vabs(m.mMat[0]),vabs(m.mMat[1])}; }
    /*  _________________________________________________________________________ */
/*! Mat22FromAngle

@param rads The angle in radians.

@return Mat22 A 2x2 matrix representing a rotation.

Creates a 2x2 rotation matrix from the given angle in radians.
*/

    Mat22 Mat22FromAngle(float rads) {
        float c = cosf(rads);
        float s = sinf(rads);
        return Mat22{
            c, s,
            -s, c
        };
    }
    /*  _________________________________________________________________________ */
/*! Mat33FromAngle

@param rads The angle in radians.

@return Mat33 A 3x3 matrix representing a rotation.

Creates a 3x3 rotation matrix from the given angle in radians.
*/

    Mat33 Mat33FromAngle(float rads) {
        float c = cosf(rads);
        float s = sinf(rads);
        return Mat33{
            c, s, 0,
            -s, c, 0,
            0, 0, 1
        };
    }
    /*  _________________________________________________________________________ */
/*! Mat33FromTranslate

@param x The x translation.
@param y The y translation.

@return Mat33 A 3x3 matrix representing a translation.

Creates a 3x3 translation matrix from the given x and y values.
*/

    Mat33 Mat33FromTranslate(float x, float y) {
        return Mat33{
            1,0,0,
            0,1,0,
            x,y,1
        };
    }
    using namespace Physics;
    //returns the min and max values of the 
    /*  _________________________________________________________________________ */
/*! GetAABBBody

@param rb A reference to a RigidBody.

@return std::pair<Vec2, Vec2> A pair representing the minimum and maximum corners of the AABB.

Computes the Axis-Aligned Bounding Box (AABB) for a given rigid body.
*/

    std::pair<Vec2, Vec2> GetAABBBody(Collider const& rb) {
        if (rb.type == ColliderType::CIRCLE) 
            return {
                rb.position - rb.dimension * .5f, rb.position + rb.dimension*.5f };
      
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
    /*  _________________________________________________________________________ */
/*! ClipSegmentToLine

@param vOut An array of ClipVertex to store the output vertices.
@param vIn An array of ClipVertex representing the input vertices.
@param normal The normal of the clipping plane.
@param offset The offset of the clipping plane.
@param clipEdge The edge number for clipping.

@return int The number of output vertices after clipping.

Clips a line segment against a plane defined by a normal and an offset.
*/

    int ClipSegmentToLine(ClipVertex vOut[2], ClipVertex vIn[2], const Vec2& normal, float offset,
        float clipEdge) {
        // Start with no output points
        int numOut = 0;

        // Calculate the distance of end points to the line
        float distance0 = dot(normal, vIn[0].v) - offset;
        float distance1 = dot(normal, vIn[1].v) - offset;

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
    /*  _________________________________________________________________________ */
/*! ComputeIncidentEdge

@param c An array of ClipVertex to store the computed incident edge.
@param h Half extents of the box.
@param pos Position of the box.
@param rot Rotation matrix of the box.
@param normal The normal against which the incident edge is computed.

@return none.

Computes the incident edge of a box against a given normal.
*/

    void ComputeIncidentEdge(ClipVertex c[2], const Vec2& h, const Vec2& pos, const Mat22& rot,
        const Vec2& normal) {
        // The normal is from the reference box. Convert it
        // to the incident boxe's frame and flip sign.

        //inverse the rotation
        Mat22 rotT = Mat22Transpose(rot);//Matrix2x2Transpose(rot);
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
    /*  _________________________________________________________________________ */
/*! Collide

@param contacts An array of Contact to store the collision contacts.
@param b1 Reference to the first RigidBody.
@param b2 Reference to the second RigidBody.

@return uint32_t The number of contact points.

Computes the collision between two rigid bodies and returns the contact points.
*/

    uint32_t BoxBoxCollide(Physics::Contact* contacts, Collider& b1, Collider& b2) {


        Vec2 h1 = b1.dimension * 0.5f;
        Vec2 h2 = b2.dimension * 0.5f;

        Vec2 pos1 = b1.position;
        Vec2 pos2 = b2.position;

        Mat22 rot1 = Mat22FromAngle(b1.rotation);
        Mat22 rot2 = Mat22FromAngle(b2.rotation);

        Mat22 rot1T = Mat22Transpose(rot1); //inverse the rotation
        Mat22 rot2T = Mat22Transpose(rot2);

        Vec2 dp = pos2 - pos1;
        Vec2 d1 = rot1T * dp;
        Vec2 d2 = rot2T * dp;

        Mat22 C = rot1T * rot2;
        Mat22 absC = mabs(C);//Matrix2x2Abs(C);
        Mat22 absCT = Mat22Transpose(absC);

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
        normal = (d1.x > 0.0f) ? rot1.mMat[0] : rot1.mMat[0] * (-1.0f);

        const float relativeToL = 0.95f;
        const float absoluteToL = 0.01f;

        if (face1.y > relativeToL * seperation + absoluteToL * h1.y) {
            axis = Axis::FACE_A_Y;
            seperation = face1.y;
            normal = (d1.y > 0.0f) ? rot1.mMat[1] : rot1.mMat[1] * (-1.0f);
        }
            
        // Box 2 faces
        if (face2.x > relativeToL * seperation + absoluteToL * h2.x) {
            axis = Axis::FACE_B_X;
            seperation = face2.x;
            normal = (d2.x > 0.0f) ? rot2.mMat[0] : rot2.mMat[0] * (-1.0f);
        }

        if (face2.y > relativeToL * seperation + absoluteToL * h2.y) {
            axis = Axis::FACE_B_Y;
            seperation = face2.y;
            normal = (d2.y > 0.0f) ? rot2.mMat[1] : rot2.mMat[1] * (-1.0f);
        }

        // Setup clipping plane data based on the separating axis
        Vec2 frontNormal{}, sideNormal{};
        ClipVertex incidentEdge[2];
        float front{}, negSide{}, posSide{};
        EdgeNumbers negEdge{}, posEdge{};

        // Compute the clipping lines and the line segment to be clipped
        switch (axis) {
        case Axis::FACE_A_X: {
            frontNormal = normal;
            front = dot(pos1, frontNormal) + h1.x;
            sideNormal = rot1.mMat[1];
            float side = dot(pos1, sideNormal);
            negSide = -side + h1.y;
            posSide = side + h1.y;
            negEdge = EdgeNumbers::EDGE3;
            posEdge = EdgeNumbers::EDGE1;
            ComputeIncidentEdge(incidentEdge, h2, pos2, rot2, frontNormal);
        } break;
        case Axis::FACE_A_Y: {
            frontNormal = normal;
            front = dot(pos1, frontNormal) + h1.y;
            sideNormal = rot1.mMat[0];
            float side = dot(pos1, sideNormal);
            negSide = -side + h1.x;
            posSide = side + h1.x;
            negEdge = EdgeNumbers::EDGE2;
            posEdge = EdgeNumbers::EDGE4;
            ComputeIncidentEdge(incidentEdge, h2, pos2, rot2, frontNormal);
        } break;
        case Axis::FACE_B_X: {
            frontNormal = normal * (-1.0f);
            front = dot(pos2, frontNormal) + h2.x;
            sideNormal = rot2.mMat[1];
            float side = dot(pos2, sideNormal);
            negSide = -side + h2.y;
            posSide = side + h2.y;
            negEdge = EdgeNumbers::EDGE3;
            posEdge = EdgeNumbers::EDGE1;
            ComputeIncidentEdge(incidentEdge, h1, pos1, rot1, frontNormal);
        } break;
        case Axis::FACE_B_Y: {
            frontNormal = normal * (-1.0f);
            front = dot(pos2, frontNormal) + h2.y;
            sideNormal = rot2.mMat[0];
            float side = dot(pos2, sideNormal);
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
            float seperation1 = dot(frontNormal, clipPoints2[i].v) - front;

            if (seperation1 <= 0) {
                contacts[numContacts].seperation = seperation1;
                contacts[numContacts].normal = normal;
                // slide contact point onto reference face (easy to cull)
                contacts[numContacts].position = clipPoints2[i].v - frontNormal * seperation1;
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

        //if (!b1.isGrounded) {
        //    b1.isGrounded = (normal.y < 0.0f);
        //}

        return numContacts;
    }
    uint32_t CircleCircleCollide(Physics::Contact* contacts, Collider& b1, Collider& b2) {
        // Calculate translational vector, which is normal
        Vec2 normal{ b2.position - b1.position };
        float b1Radius{ b1.dimension.x * .5f }, b2Radius{ b2.dimension.x * .5f };
        float dist_sqr{ Image::dot(normal, normal) };
        float radius { b1Radius + b2Radius };

        // Not in contact
        if (dist_sqr >= radius * radius)
        {
            return 0;
        }

        float distance = std::sqrt(dist_sqr);

        if (distance == 0.0f)
        {
            contacts[0].seperation = -b1Radius;
            contacts[0].normal = Vec2(1, 0);
            contacts[0].position = b1.position;
        }
        else
        {
            contacts[0].seperation = radius - distance;
            contacts[0].normal = normal / distance; // Faster than using Normalized since we already performed sqrt
            contacts[0].position = normal * b1Radius + b1.position;
        }
        return 1;
    }
    uint32_t CircleBoxCollide(Physics::Contact* contacts, Collider& b1, Collider& b2) {
        float b1Radius{ b1.dimension.x * .5f };
        uint32_t contact_count{};
        Mat22 b2rot{};
        Image::Mat22RotRad(b2rot, b2.rotation);
        // Transform circle center to Polygon model space
        Vec2 center = b1.position;
        //center = b2invrot * (center - b2.position);

        // Find edge with minimum penetration
        // Exact concept as using support points in Polygon vs Polygon
        float separation = -FLOAT_MAX;
        uint32_t faceNormal = 0;

        std::array<Vec2, 4> b2Normals{
            b2rot* Vec2{ -1, 0 }, //left
            b2rot* Vec2{ 0,1 }, //top
            b2rot* Vec2{ 1, 0 }, //right 
            b2rot* Vec2{ 0,-1 }, //bottom
        };
        std::array<Vec2, 4> b2Vertices{
            b2.position + ((b2Normals[0] + b2Normals[3]) * (b2.dimension * .5f)), //btm left
            b2.position + ((b2Normals[0] + b2Normals[1]) * (b2.dimension * .5f)), //top left
            b2.position + ((b2Normals[2] + b2Normals[1]) * (b2.dimension * .5f)), //top right
            b2.position + ((b2Normals[2] + b2Normals[3]) * (b2.dimension * .5f)) //btm right       
        };

        for (uint32_t i = 0; i < b2Normals.size(); ++i)
        {
            float s = Image::dot(b2Normals[i], center - b2Vertices[i]);

            if (s > b1Radius)
                return 0;

            if (s > separation)
            {
                separation = s;
                faceNormal = i;
            }
        }

        // Grab face's vertices
        Vec2 v1 = b2Vertices[faceNormal];
        uint32_t i2 = (faceNormal + 1 < b2Vertices.size()) ? faceNormal + 1 : 0;
        Vec2 v2 = b2Vertices[i2];

        // Check to see if center is within polygon
        if (separation < FLT_EPSILON)
        {
            contact_count = 1;
            contacts[0].normal = -b2Normals[faceNormal];
            contacts[0].position = contacts[0].normal * b1Radius + b1.position;
            contacts[0].seperation = -b1Radius;
            return contact_count;
        }

        // Determine which voronoi region of the edge center of circle lies within
        float dot1 = Image::dot(center - v1, v2 - v1);
        float dot2 = Image::dot(center - v2, v1 - v2);
        contacts[0].seperation = -(b1Radius - separation);

        // Closest to v1
        if (dot1 <= 0.0f)
        {
            Vec2 dist(center - v1);
            if (Image::dot(dist, dist) > b1Radius * b1Radius)
                return 0;

            contact_count = 1;
            Vec2 n = v1 - center;
            //n = b2.u * n;
           
            contacts[0].normal = Image::normalized(n);
            //v1 = b2.u * v1 + b2.position;
            contacts[0].position = v1;
        }

        // Closest to v2
        else if (dot2 <= 0.0f)
        {
            Vec2 dist(center - v2);
            if (Image::dot(dist, dist) > b1Radius * b1Radius)
                return 0;

            contact_count = 1;
            Vec2 n = v2 - center;
            //v2 = b2.u * v2 + b2.position;
            //m->contacts[0] = v2;
            contacts[0].position = v2;
            //n = b2.u * n;
            
            contacts[0].normal = Image::normalized(n);
        }

        // Closest to face
        else
        {
            Vec2 n = b2Normals[faceNormal];
            if (Image::dot(center - v1, n) > b1Radius)
                return 0;

            //n = b2.u * n;
            contacts[0].normal = -n;
            contacts[0].position = contacts[0].normal * b1Radius + b1.position;
            contact_count = 1;
        }
        return contact_count;
    }
    uint32_t BoxCircleCollide(Physics::Contact* contacts, Collider& b1, Collider& b2) {
        uint32_t out{ CircleBoxCollide(contacts, b2, b1) };
        contacts[0].normal = -(contacts[0].normal);
        return out;
    }
    /*  _________________________________________________________________________ */
/*! Collide

@param b1 The first Entity.
@param b2 The second Entity.

@return Arbiter The collision arbiter between the two entities.

Computes the collision between two entities and returns an arbiter.
*/

    Arbiter CollisionSystem::Collide(Entity b1, Entity b2) {
        auto & rb1{ Coordinator::GetInstance()->GetComponent<Collider>(b1) };
        auto & rb2{ Coordinator::GetInstance()->GetComponent<Collider>(b2) };

        auto aabb1{ GetAABBBody(rb1) };
        auto aabb2{ GetAABBBody(rb2) };
        //////trivial reject
        if (!CheckAABBDiscrete(aabb1.first, aabb1.second, aabb2.first, aabb2.second)) {
            //std::cout << "reject\n";
            return Arbiter{};
        }
        auto& c1{ Coordinator::GetInstance()->GetComponent<Collider>(b1) };
        auto& c2{ Coordinator::GetInstance()->GetComponent<Collider>(b2) };

        Arbiter result {};

        result.b1 = b1;
        result.b2 = b2;

        //result.combinedFriction = sqrtf(rb1.friction * rb2.friction);
        result.contactsCount = mLookupTable[ColliderLookupKey{c1.type, c2.type}](result.contacts, rb1, rb2);

        return result;
    }

    //raycast obb or circle
    bool CollisionSystem::RaycastBody(Vec2 const& origin, Vec2 const& end, Entity e, Vec2& cn, Vec2& cp, float& time) {
        auto const& collider{ gCoordinator->GetComponent<Collider>(e) };
        if (collider.type == ColliderType::BOX) {
            Mat33 rot{}, translate{}, invrot{}, invtranslate{};
            Image::Mat33RotRad(rot, collider.rotation);
            Image::Mat33Translate(translate, collider.position.x, collider.position.y);
            Image::Mat33RotRad(invrot, -collider.rotation);
            Image::Mat33Translate(invtranslate, -collider.position.x, -collider.position.y);
            Mat33 inverse{invrot * invtranslate}, transform{translate * rot};
            Vec3 trnsOrigin{ inverse * Vec3{origin.x, origin.y, 1} };
            bool out{ CheckRayRect(
                Vec2{trnsOrigin}, Vec2{Vec3(inverse * Vec3{end.x, end.y, 1}) - trnsOrigin},
                CollisionRect(-collider.dimension / 2.f, collider.dimension),
                cp, cn, time
                ) };

            if (out) {
                cn = std::move(Vec2{ rot * Vec3{ cn.x, cn.y, 1 } });
                cp = std::move(Vec2{transform * Vec3{ cp.x, cp.y, 1 }});
            
            }

            return out;
        }
        return false;
    }

    bool CollisionSystem::Raycast(Vec2 const& origin, Vec2 const& end, RayHit& rh, Entity entityToIgnore) {
        float timeMin{ FLOAT_MAX };
        Entity eMin{};
        Vec2 cnMin{0, 0}, cpMin{0, 0};
        bool out{ false };
        for (auto const& entity : mEntities) {
            if (entityToIgnore != static_cast<Entity>(-1) && entityToIgnore == entity) continue;
            //if (entity == 0) continue;
            float time{};
            Vec2 cn{}, cp{};
            if (RaycastBody(origin, end, entity, cn, cp, time)) {
              //out = true;
              if (timeMin > time && time <= 1.f) {
                timeMin = std::move(time);
                eMin = entity;
                cnMin = std::move(cn);
                cpMin = std::move(cp);
                out = true;
              }
            }
        }
        rh = RayHit{
            cnMin, cpMin, timeMin, eMin
        };

        glm::vec4 col{ (!out) ? glm::vec4(0.f, 1.f, 0.f, 1.f) : glm::vec4(1.f, 0.f, 0.f, 1.f) };
        std::pair<std::pair<Vec2, Vec2>, glm::vec4> beginEnd{ {origin, end}, col };
        Event event{ Events::Physics::Raycast::RAYCAST_FIRED };
        event.SetParam(Events::Physics::Raycast::Debug::RAYCAST_DEBUGGED, beginEnd);
        gCoordinator->SendEvent(event);

        return out;
    }

  /*  _________________________________________________________________________ */
/*! CollisionSystem::Init

@return none.

Initializes the CollisionSystem, setting up the Quadtree and other necessary components.
*/

    void CollisionSystem::Init() {
        gCoordinator = Coordinator::GetInstance();
        using namespace DataMgmt;
        mQuadtree = DataMgmt::Quadtree<Entity>{ 0, Rect(Vec2(static_cast<float>(-6000), static_cast<float>(-6000)), Vec2(static_cast<float>(6000), static_cast<float>(6000)))};
        mLookupTable[ColliderLookupKey{ ColliderType::BOX, ColliderType::BOX }] = BoxBoxCollide;
        mLookupTable[ColliderLookupKey{ ColliderType::CIRCLE, ColliderType::BOX }] = CircleBoxCollide;
        mLookupTable[ColliderLookupKey{ ColliderType::BOX, ColliderType::CIRCLE }] = BoxCircleCollide;
        mLookupTable[ColliderLookupKey{ ColliderType::CIRCLE, ColliderType::CIRCLE }] = CircleCircleCollide;

        ::gCoordinator->AddEventListener(FUNCTION_LISTENER(Events::Physics::Raycast::RAYCAST_FIRED, RenderSystem::DebugRay));
    }
    /*  _________________________________________________________________________ */
/*! CollisionSystem::Update

@param dt The delta time since the last frame.

@return none.

Updates the CollisionSystem, checking for collisions between entities and sending collision events.
*/

    void CollisionSystem::Update(float dt) {
        UNREFERENCED_PARAMETER(dt);
        //for (auto const& entity : mEntities) {
        //	auto& collider = gCoordinator->GetComponent<Collider>(entity);
        //	auto& rigidBody = gCoordinator->GetComponent<RigidBody>(entity);


        //}
        mCollidedEntities.clear();

        mQuadtree.Update(mEntities, [](Entity const& e, DataMgmt::Rect const& r) {
            auto const& rigidBody{ Coordinator::GetInstance()->GetComponent<Collider>(e) };
            
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
                    
                    auto const& layer1 = gCoordinator->GetComponent<Layering>(entityVec[i]).assignedLayer;
                    auto const& layer2 = gCoordinator->GetComponent<Layering>(entityVec[j]).assignedLayer;
                    if (!LayeringSystem::IsCollidable(layer1, layer2)) continue;
                    
                    Arbiter arbiter = Collide(entityVec[i], entityVec[j]);
                    ArbiterKey arbiterKey{ entityVec[i], entityVec[j] };
                    uint64_t hashTableKey = murmur64((void*)&arbiterKey, sizeof(ArbiterKey));

                    if (arbiter.contactsCount > 0) {
                        Event event{ Events::Physics::COLLISION };
                        event.SetParam(Events::Physics::Collision::COLLIDED, ArbiterPair {hashTableKey, arbiter});
                        gCoordinator->SendEvent(event);

                        //updates 
                        mCollidedEntities[arbiter.b1].emplace_back(std::move(arbiter));
                    }


                }
            }
        }
    }
    /*  _________________________________________________________________________ */
/*! CollisionSystem::Debug

@return none.

Debugs the CollisionSystem, drawing AABBs and other debug information.
*/

    void CollisionSystem::Debug() {
        mQuadtree.Debug();
        //auto& camera = Coordinator::GetInstance()->GetComponent<OrthoCamera>(Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetCamera());
        //Renderer::RenderSceneBegin(camera);
        //size_t sizeent{ mEntities.size() };
        //RayHit rh;
        //Renderer::DrawLine({ Testing::testingStart.x,Testing::testingStart.y, 0.f }, { Testing::testingEnd.x,Testing::testingEnd.y , 1 }, { 1,0,0,1 });
        /*
        if (Raycast(Testing::testingStart, Testing::testingEnd, rh)) {
            //std::cout << rh.entityID << std::endl;
            Renderer::DrawLine({ rh.point.x,rh.point.y, 0.f }, { rh.point.x + rh.normal.x * 50.f,rh.point.y + rh.normal.y * 50.f , 1 }, { 1,0,0,1 });

        }
        */
        for (auto const& e : mEntities) {
            //auto const& rb{ Coordinator::GetInstance()->GetComponent<RigidBody>(e) };
            auto const& c{ Coordinator::GetInstance()->GetComponent<Collider>(e) };
            auto aabb{ GetAABBBody(c) };
            auto scale{ aabb.second - aabb.first };
            Vec2 pos{ aabb.first + scale / 2.f };
            //Vec2 p1{ c.position + rb.velocity };
            if (c.type == ColliderType::BOX) {
                //Renderer::DrawQuad({ c.position.x,c.position.y,1 }, { c.dimension.x,c.dimension.y }, { 1.f, 1.f, 1.f ,0.f }, Image::Degree(c.rotation));
                Renderer::DrawLineRect({ c.position.x,c.position.y,1 }, { c.dimension.x,c.dimension.y }, { 1.f, 0.f, 1.f ,1.f }, Image::Degree(c.rotation));
                //Renderer::DrawLine({ rb.position.x,rb.position.y, 0.f }, { p1.x,p1.y , 1 }, { 0,1,0,1 });
            }
            else {
                Renderer::DrawCircle({ c.position.x,c.position.y, 0.f }, { c.dimension.x, c.dimension.x }, { 1.f,1.f,1.f, 1.f }, .1f);
            }
            Renderer::DrawLineRect({ pos.x,pos.y,1 }, { scale.x,scale.y }, { 1.f, 1.f, 1.f ,1.f });
            //Renderer::DrawLine({ rb.position.x,rb.position.y, 0.f }, {p1.x,p1.y , 1 }, { 0,1,0,1 });

            ArbiterVec av{ Physics::IsIntersected(e) };
            for (auto const& a : av) {
                for (uint64_t i{}; i < a.contactsCount; ++i) {
                    Renderer::DrawCircle({ a.contacts[i].position.x, a.contacts[i].position.y, 10 }, { 5,5 }, { 0,1,1,1 });
                }
            }
            ArbiterVec av2{ Physics::IsCollided(e) };
            for (auto const& a : av2) {
                for (uint64_t i{}; i < a.contactsCount; ++i) {
                    Renderer::DrawCircle({ a.contacts[i].position.x, a.contacts[i].position.y, 10 }, { 5,5 }, { 1,0,1,1 });
                    Renderer::DrawLine({ a.contacts[i].position.x, a.contacts[i].position.y, 10 },
                    {
                        a.contacts[i].position.x + a.contacts[i].normal.x * 2.f,
                        a.contacts[i].position.y + a.contacts[i].normal.y * 2.f,
                        10
                    }, { 1,1,0,1 });
                }
            }

        }
        //2 is the id of the player
        //ArbiterVec av{ Physics::IsCollided(2) };

        //Renderer::RenderSceneEnd();

    }

    bool CollisionSystem::IsIntersected(Entity const& e1, Entity const& e2) {
        if (mCollidedEntities.find(e1) != mCollidedEntities.end()) return false;
        auto it{ std::find_if(mCollidedEntities[e1].begin(), mCollidedEntities[e1].end(), [e2](Arbiter const& a) {
            return a.b2 == e2;
        }) };
        if (it == mCollidedEntities[e1].end()) return false;
        return true;
    }
    bool CollisionSystem::IsIntersected(Entity const& e1, Entity const& e2, Arbiter& a) {
        if (mCollidedEntities.find(e1) != mCollidedEntities.end()) return false;
        auto it{ std::find_if(mCollidedEntities[e1].begin(), mCollidedEntities[e1].end(), [e2](Arbiter const& a) {
            return a.b2 == e2;
        }) };
        if (it == mCollidedEntities[e1].end()) return false;
        a = *it;
        return true;
    }

    ArbiterVec CollisionSystem::IsIntersected(Entity const& e1) {
        if (mCollidedEntities.find(e1) == mCollidedEntities.end()) return ArbiterVec{};
        return mCollidedEntities[e1];
    }
}
