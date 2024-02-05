#pragma once
#include <Engine/States/State.hpp>
#include <memory>
#include <Systems/PhysicsSystem.hpp>
#include <Systems/CollisionSystem.hpp>
#include <Systems/EditorControlSystem.hpp>
#include <Systems/AnimationSystem.hpp>
#include <Systems/RenderSystem.hpp>
#include <Systems/EntitySerializationSystem.hpp>
#include <Systems/ParticleSystem.hpp>

#include <Core/Component.hpp>

class MainState : public State {
public:
	void Init() override;
	void Exit() override;

	void Update(float dt) override; 
	void Render(float dt) override;
private:
};