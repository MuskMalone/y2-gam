#include "Systems/PhysicsSystem.hpp"

#include "Components/Gravity.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Core/Coordinator.hpp"
#include <Components/BoxCollider.hpp>
#include <memory>
#include <glm/matrix.hpp>
#include <Math/Vec2.hpp>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <Core/Physics.hpp>
#include <Core/Types.hpp>

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}
namespace Physics {
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
    void ArbiterPreStep(Arbiter & a, float inv_dt) {
        const float k_allowed_penetration = 0.0f;
        float k_bias_factor = .2f;

        auto& rb1{ Coordinator::GetInstance()->GetComponent<RigidBody>(a.b1) };
        auto& rb2{ Coordinator::GetInstance()->GetComponent<RigidBody>(a.b2) };


        for (size_t i = 0; i < a.contacts_count; i++) {
            Contact& c = *(a.contacts + i);

            Vec2 r1 = c.position - rb1.position;
            Vec2 r2 = c.position - rb2.position;

            // Precompute normal mass, tangent mass, and bias
            float rn1 = glm::dot(r1, c.normal);
            float rn2 = glm::dot(r2, c.normal);
            float k_normal = rb1.invMass + rb2.invMass;
            k_normal += rb1.invInertia * (glm::dot(r1, r1) - rn1 * rn1)
                + rb2.invInertia * (glm::dot(r2, r2) - rn2 * rn2);
            c.mass_normal = 1.0f / k_normal;

            Vec2 tangent = Vector2Cross(c.normal, 1.0f);
            float rt1 = glm::dot(r1, tangent);
            float rt2 = glm::dot(r2, tangent);
            float k_tangent = rb1.invMass + rb2.invMass;
            k_tangent += rb1.invInertia * (glm::dot(r1, r1) - rt1 * rt1)
                + rb2.invInertia * (glm::dot(r2, r2) - rt2 * rt2);
            c.mass_tangent = 1.0f / k_tangent;

            c.bias = -k_bias_factor * inv_dt * std::min(0.0f, c.seperation + k_allowed_penetration);

            // accumulate impulses
            {
                Vec2 P = c.normal * c.acc_normal_impulse + tangent * c.acc_tangent_impulse;

                rb1.velocity -= P * rb1.invMass;
                if (!rb1.isLockRotation) {
                    rb1.angularVelocity -= rb1.invInertia * Vector2Cross(r1, P);
                }

                rb2.velocity += P * rb2.invMass;
                if (!rb1.isLockRotation) {
                    rb2.angularVelocity += rb2.invInertia * Vector2Cross(r2, P);
                }
            }
        }
    }

    void ArbiterApplyImpulse(Arbiter& a) {
        auto& rb1{ Coordinator::GetInstance()->GetComponent<RigidBody>(a.b1) };
        auto& rb2{ Coordinator::GetInstance()->GetComponent<RigidBody>(a.b2) };


        for (size_t i = 0; i < a.contacts_count; i++) {
            Contact& c = *(a.contacts + i);
            c.r1 = c.position - rb1.position;
            c.r2 = c.position - rb2.position;

            // Relative velocity at contact
            Vec2 dv = rb2.velocity + Vector2Cross(rb2.angularVelocity, c.r2) - rb1.velocity
                - Vector2Cross(rb1.angularVelocity, c.r1);

            // Compute normal impulse
            float vn = glm::dot(dv, c.normal);

            float dPn = c.mass_normal * (-vn + c.bias);

            // Clamp the accumulated impulse
            float Pn0 = c.acc_normal_impulse;
            c.acc_normal_impulse = std::max(Pn0 + dPn, 0.0f);
            dPn = c.acc_normal_impulse - Pn0;

            // Apply contact impulse
            Vec2 Pn = c.normal * dPn;

            rb1.velocity -= Pn * rb1.invMass;
            if (!rb1.isLockRotation) {
                rb1.angularVelocity -= rb1.invInertia * Vector2Cross(c.r1, Pn);
            }

            rb2.velocity += Pn * rb2.invMass;
            if (!rb2.isLockRotation) {
                rb2.angularVelocity += rb2.invInertia * Vector2Cross(c.r2, Pn);
            }

            // Relative velocity at contact
            dv = rb2.velocity + Vector2Cross(rb2.angularVelocity, c.r2) - rb1.velocity
                - Vector2Cross(rb1.angularVelocity, c.r1);

            Vec2 tangent = Vector2Cross(c.normal, 1.0f);
            float vt = glm::dot(dv, tangent);
            float dPt = vt * c.mass_tangent * (-1.0f);

            // accumulate impulses
            {
                // Compute frictional impulse
                float maxPt = a.combined_friction * c.acc_normal_impulse;
                // Clamp friction
                float old_tangent_impulse = c.acc_tangent_impulse;
                c.acc_tangent_impulse = glm::clamp(old_tangent_impulse + dPt, -maxPt, maxPt);
                dPt = c.acc_tangent_impulse - old_tangent_impulse;
            }

            // Apply contact impulse
            Vec2 Pt = tangent * dPt;
            
            rb1.velocity -= Pt * rb1.invMass;
            if (!rb1.isLockRotation) {
                rb1.angularVelocity -= rb1.invInertia * Vector2Cross(c.r1, Pt);
            }

            rb2.velocity += Pt * rb2.invMass;
            if (!rb2.isLockRotation) {
                rb2.angularVelocity += rb2.invInertia * Vector2Cross(c.r2, Pt);
            }
        }
    }

	void PhysicsSystem::Init()
	{
		gCoordinator = Coordinator::GetInstance();
        ::gCoordinator->AddEventListener(METHOD_LISTENER(Events::Physics::COLLISION, PhysicsSystem::CollisionListener));

        mArbiterTable.clear();
	}


    void PhysicsSystem::PreCollisionUpdate(float dt)
    {
        mArbiterTable.clear();
        for (auto const& entity : mEntities)
        {
            auto& rigidBody = gCoordinator->GetComponent<RigidBody>(entity);
            auto& collider = gCoordinator->GetComponent<BoxCollider>(entity);
            rigidBody.isGrounded = false;
        }
    }

	void PhysicsSystem::PostCollisionUpdate(float dt) {
        // Integrate forces
        float invDt{ 1.f / dt };
        for (auto const& entity : mEntities)
        {
            auto& rigidBody = gCoordinator->GetComponent<RigidBody>(entity);
            auto& transform = gCoordinator->GetComponent<Transform>(entity);
            auto& collider = gCoordinator->GetComponent<BoxCollider>(entity);
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
            auto& collider = gCoordinator->GetComponent<BoxCollider>(entity);

            rigidBody.position += rigidBody.velocity * dt;
            rigidBody.rotation += rigidBody.angularVelocity * dt;

            //change this soon
            transform.position = Vec3{rigidBody.position, 0};
            transform.rotation = Vec3{ 0, 0, glm::degrees(rigidBody.rotation) };

            rigidBody.torque = 0.0f;
            rigidBody.force = Vec2{};//Vector2Zero();
        }

	}
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
