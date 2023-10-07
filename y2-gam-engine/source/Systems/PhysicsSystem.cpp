/******************************************************************************/
/*!
\par        Image Engine
\file       Physics.cpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		for physics

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "../include/pch.hpp"

#include "Systems/PhysicsSystem.hpp"
#include "Components/Gravity.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Core/Coordinator.hpp"
#include <Components/BoxCollider.hpp>
#include <Core/Physics.hpp>
#include <Core/Types.hpp>
#include "Math/MathUtils.h"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}
namespace Physics {
    /*  _________________________________________________________________________ */
    /*! ArbiterMergeContacts

    @param a The primary arbiter whose contacts might be updated.
    @param toMerge The arbiter containing new contacts to be merged.

    Merges contact points between two arbiters. This function checks if the new
    contacts from `toMerge` already exist in `a`. If they do, it updates the
    accumulated impulses; otherwise, it simply copies the new contact.
    */

    void ArbiterMergeContacts(Arbiter& a, Arbiter toMerge) {
        Contact mergedContacts[2];

        for (uint32_t i = 0; i < toMerge.contactsCount; i++) {
            Contact& cNew{ toMerge.contacts[i] };
            int k = -1;
            for (uint32_t j = 0; j < a.contactsCount; j++) {
                Contact& cOld = *(a.contacts + j);
                if (cNew.feature.value == cOld.feature.value) {
                    k = j;
                    break;
                }
            }

            if (k > -1) {
                Contact& c{ mergedContacts[i] };
                Contact& cOld{ a.contacts[k] };
                c = cNew;

                // Warm starting
                c.accNormalImpulse = cOld.accNormalImpulse;
                c.accTangentImpulse = cOld.accTangentImpulse;
                c.accBiasedNormalImpulse = cOld.accBiasedNormalImpulse;
            }
            else {
                mergedContacts[i] = toMerge.contacts[i];
            }
        }

        for (uint32_t i = 0; i < toMerge.contactsCount; i++) {
            a.contacts[i] = mergedContacts[i];
        }

        a.contactsCount = toMerge.contactsCount;
    }
    /*  _________________________________________________________________________ */
/*! ArbiterPreStep

@param a The arbiter to be prepared.
@param inv_dt The inverse of the time step (i.e., 1/dt).

Prepares the arbiter for the impulse resolution phase. Computes the normal
and tangent mass for each contact point, which will be used to calculate the
impulses applied during collision resolution. Also precomputes some values
for the impulse application phase.
*/

    void ArbiterPreStep(Arbiter & a, float inv_dt) {
        const float kAllowedPenetration = 0.0f;
        float kBiasFactor = .2f;

        auto& rb1{ Coordinator::GetInstance()->GetComponent<RigidBody>(a.b1) };
        auto& rb2{ Coordinator::GetInstance()->GetComponent<RigidBody>(a.b2) };


        for (size_t i = 0; i < a.contactsCount; i++) {
            Contact& c = *(a.contacts + i);

            Vec2 r1 = c.position - rb1.position;
            Vec2 r2 = c.position - rb2.position;

            // Precompute normal mass, tangent mass, and bias
            float rn1 = dot(r1, c.normal);
            float rn2 = dot(r2, c.normal);
            float kNormal = rb1.invMass + rb2.invMass;
            kNormal += rb1.invInertia * (dot(r1, r1) - rn1 * rn1)
                + rb2.invInertia * (dot(r2, r2) - rn2 * rn2);
            c.massNormal = 1.0f / kNormal;

            Vec2 tangent = cross(c.normal, 1.0f);
            float rt1 = dot(r1, tangent);
            float rt2 = dot(r2, tangent);
            float kTangent = rb1.invMass + rb2.invMass;
            kTangent += rb1.invInertia * (dot(r1, r1) - rt1 * rt1)
                + rb2.invInertia * (dot(r2, r2) - rt2 * rt2);
            c.massTangent = 1.0f / kTangent;

            c.bias = -kBiasFactor * inv_dt * std::min(0.0f, c.seperation + kAllowedPenetration);

            // accumulate impulses
            {
                Vec2 P = c.normal * c.accNormalImpulse + tangent * c.accTangentImpulse;

                rb1.velocity -= P * rb1.invMass;
                if (!rb1.isLockRotation) {
                    rb1.angularVelocity -= rb1.invInertia * cross(r1, P);
                }

                rb2.velocity += P * rb2.invMass;
                if (!rb1.isLockRotation) {
                    rb2.angularVelocity += rb2.invInertia * cross(r2, P);
                }
            }
        }
    }
    /*  _________________________________________________________________________ */
/*! ArbiterApplyImpulse

@param a The arbiter whose colliding bodies will have impulses applied.

Applies impulses to the colliding bodies based on their relative velocities.
Ensures that the relative velocity along the contact normal becomes zero after
the impulse is applied, preventing the bodies from penetrating each other.
*/

    void ArbiterApplyImpulse(Arbiter& a) {
        auto& rb1{ Coordinator::GetInstance()->GetComponent<RigidBody>(a.b1) };
        auto& rb2{ Coordinator::GetInstance()->GetComponent<RigidBody>(a.b2) };


        for (size_t i = 0; i < a.contactsCount; i++) {
            Contact& c = *(a.contacts + i);
            c.r1 = c.position - rb1.position;
            c.r2 = c.position - rb2.position;

            // Relative velocity at contact
            Vec2 dv = rb2.velocity + cross(rb2.angularVelocity, c.r2) - rb1.velocity
                - cross(rb1.angularVelocity, c.r1);

            // Compute normal impulse
            float vn = dot(dv, c.normal);

            float dPn = c.massNormal * (-vn + c.bias);

            // Clamp the accumulated impulse
            float Pn0 = c.accNormalImpulse;
            c.accNormalImpulse = std::max(Pn0 + dPn, 0.0f);
            dPn = c.accNormalImpulse - Pn0;

            // Apply contact impulse
            Vec2 Pn = c.normal * dPn;

            rb1.velocity -= Pn * rb1.invMass;
            if (!rb1.isLockRotation) {
                rb1.angularVelocity -= rb1.invInertia * cross(c.r1, Pn);
            }

            rb2.velocity += Pn * rb2.invMass;
            if (!rb2.isLockRotation) {
                rb2.angularVelocity += rb2.invInertia * cross(c.r2, Pn);
            }

            // Relative velocity at contact
            dv = rb2.velocity + cross(rb2.angularVelocity, c.r2) - rb1.velocity
                - cross(rb1.angularVelocity, c.r1);

            Vec2 tangent = cross(c.normal, 1.0f);
            float vt = dot(dv, tangent);
            float dPt = vt * c.massTangent * (-1.0f);

            // accumulate impulses
            {
                // Compute frictional impulse
                float maxPt = a.combinedFriction * c.accNormalImpulse;
                // Clamp friction
                float oldTangentImpulse = c.accTangentImpulse;
                c.accTangentImpulse = std::clamp(oldTangentImpulse + dPt, -maxPt, maxPt);
                dPt = c.accTangentImpulse - oldTangentImpulse;
            }

            // Apply contact impulse
            Vec2 Pt = tangent * dPt;
            
            rb1.velocity -= Pt * rb1.invMass;
            if (!rb1.isLockRotation) {
                rb1.angularVelocity -= rb1.invInertia * cross(c.r1, Pt);
            }

            rb2.velocity += Pt * rb2.invMass;
            if (!rb2.isLockRotation) {
                rb2.angularVelocity += rb2.invInertia * cross(c.r2, Pt);
            }
        }
    }
    /*  _________________________________________________________________________ */
/*! PhysicsSystem::Init

Initializes the physics system. Sets up an event listener for collision events
and clears the arbiter table.
*/

	void PhysicsSystem::Init()
	{
		gCoordinator = Coordinator::GetInstance();
        ::gCoordinator->AddEventListener(METHOD_LISTENER(Events::Physics::COLLISION, PhysicsSystem::CollisionListener));

        mArbiterTable.clear();
	}

    /*  _________________________________________________________________________ */
/*! PhysicsSystem::PreCollisionUpdate

@param dt The time step for the current frame (not used in the current implementation).

Prepares the physics system for the collision detection phase. Clears the
arbiter table and resets the `isGrounded` flag for all rigid bodies.
*/

    void PhysicsSystem::PreCollisionUpdate(float dt)
    {
        UNREFERENCED_PARAMETER(dt);
        mArbiterTable.clear();
        for (auto const& entity : mEntities)
        {
            auto& rigidBody = gCoordinator->GetComponent<RigidBody>(entity);
            rigidBody.isGrounded = false;
        }
    }
    /*  _________________________________________________________________________ */
/*! PhysicsSystem::PostCollisionUpdate

@param dt The time step for the current frame.

Updates the physics system after the collision detection phase. Integrates
forces to update velocities, prepares for impulse resolution, iteratively
applies impulses, and then integrates velocities again to update positions
and rotations of the rigid bodies.
*/

	void PhysicsSystem::PostCollisionUpdate(float dt) {
        // Integrate forces
        float invDt{ 1.f / dt };
        for (auto const& entity : mEntities)
        {
            auto& rigidBody = gCoordinator->GetComponent<RigidBody>(entity);
            auto const& gravity = gCoordinator->GetComponent<Gravity>(entity);
            if (rigidBody.invMass == 0.0f) {
                continue;
            }
            rigidBody.velocity += (gravity.force + rigidBody.force * rigidBody.invMass) * dt;
            rigidBody.angularVelocity += (rigidBody.torque * rigidBody.invInertia) * dt;
        }

        // Perform pre-steps
        // for (size_t i = 0; i < mArbiterTable.entries_count; i++) {
          // for (size_t i = 0; i < mArbiterTable.size(); i++) {
          // Arbiter *a = &mArbiterTable.entries[i].value;
        for (auto& a : mArbiterTable) {
            ArbiterPreStep(a.second, invDt);
        }

        // Perform iterations
        for (size_t i = 0; i < iterations; i++) {
            // for (size_t ai = 0; ai < mArbiterTable.entries_count; ai++) {
            //   Arbiter *a = &mArbiterTable.entries[ai].value;
            //   ArbiterApplyImpulse(a);
            // }
            for (auto& a : mArbiterTable) {
                ArbiterApplyImpulse(a.second);
            }
        }

        // Integrate velocities
        for (auto const& entity : mEntities){
            auto& rigidBody = gCoordinator->GetComponent<RigidBody>(entity);
            auto& transform = gCoordinator->GetComponent<Transform>(entity);

            rigidBody.position += rigidBody.velocity * dt;
            rigidBody.rotation += rigidBody.angularVelocity * dt;

            //change this soon
            transform.position = {rigidBody.position.x,rigidBody.position.y, 0};
            transform.rotation = { 0, 0, Degree(rigidBody.rotation) };
           

            rigidBody.torque = 0.0f;
            rigidBody.force = Vec2{};//Vector2Zero();
        }

	}

    /*  _________________________________________________________________________ */
/*! PhysicsSystem::CollisionListener

@param event The event containing collision data.

Event listener function that gets called when a collision is detected. Checks
if an arbiter for the colliding pair already exists in the arbiter table. If
it does, it merges the contacts; otherwise, it adds a new arbiter to the table.
*/

    void PhysicsSystem::CollisionListener(Event& event) {
        auto const& ap{ event.GetParam<ArbiterPair>(Events::Physics::Collision::COLLIDED) };
        ArbiterHashTable::iterator iter = mArbiterTable.find(ap.first);
        //HashTableGet(&world->arbiter_table, hashTableKey);
        if (iter == mArbiterTable.end()) {
            mArbiterTable.emplace(ap.first, ap.second);
        }
        else {
            ArbiterMergeContacts(iter->second, ap.second);
        }
    }
}
