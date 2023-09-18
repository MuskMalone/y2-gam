#pragma once
#include <Engine/States/State.hpp>

class MainState : public GameState {
public:
	void Init() override;
	void Exit() override;

	void Update(float dt) override;
	void Render(float dt) override;
};