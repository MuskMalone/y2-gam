#include <Engine/StateManager.hpp>

std::shared_ptr<StateManager> StateManager::_mSelf = 0;
std::shared_ptr<StateManager> StateManager::GetInstance() {
	if (!_mSelf) return _mSelf = std::make_shared<StateManager>();
	return _mSelf;
}

void StateManager::PopState() {
	if (mStates.empty()) return;
	mStates.top()->Exit();
	mStates.pop();
	if (mStates.empty()) return;
	mStates.top()->Init();
}
void StateManager::Update(float dt) {
	if (mStates.empty()) return;
	mStates.top()->Update(dt);
}
void StateManager::Render(float dt) {
	if (mStates.empty()) return;
	mStates.top()->Render(dt);
}

void StateManager::Clear() {
	while (!mStates.empty()) {
		mStates.top()->Exit();
		mStates.pop();
	}
}