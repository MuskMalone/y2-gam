#pragma once
#include <stack>
#include <Engine/States/State.hpp>
#include <memory>

class GameStateManager {
public:
	static std::shared_ptr<GameStateManager> GetInstance();
	void PushState(std::unique_ptr<GameState> state);
	void PopState();
	void Update(float dt);
	void Render(float dt);

private:
	std::stack<std::unique_ptr<GameState>> mStates;
	static std::shared_ptr<GameStateManager> _mSelf;
};