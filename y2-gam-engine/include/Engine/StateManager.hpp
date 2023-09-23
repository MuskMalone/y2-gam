#pragma once
#include <stack>
#include <Engine/States/State.hpp>
#include <memory>

class StateManager {
public:
	static std::shared_ptr<StateManager> GetInstance();
	void PushState(std::unique_ptr<GameState> state);
	void PopState();
	void Update(float dt);
	void Render(float dt);
	void Clear();

private:
	std::stack<std::unique_ptr<GameState>> mStates;
	static std::shared_ptr<StateManager> _mSelf;
};