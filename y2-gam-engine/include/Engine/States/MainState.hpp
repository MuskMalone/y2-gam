#pragma once
#include <Engine/States/State.hpp>
#include <memory>
#include <Systems/PhysicsSystem.hpp>
#include <Systems/CollisionSystem.hpp>
#include <Systems/EditorControlSystem.hpp>
#include <Systems/AnimationSystem.hpp>
#include <Systems/RenderSystem.hpp>

#include <Components/Component.hpp>

class MainState : public GameState {
public:
	void Init() override;
	void Exit() override;

	void Update(float dt) override;
	void Render(float dt) override;

private:
	std::shared_ptr<Physics::PhysicsSystem> mPhysicsSystem;
	std::shared_ptr<Collision::CollisionSystem> mCollisionSystem;
	std::shared_ptr<EditorControlSystem> mEditorControlSystem;
	std::shared_ptr<RenderSystem> mRenderSystem;
	std::shared_ptr<AnimationSystem> mAnimationSystem;

};