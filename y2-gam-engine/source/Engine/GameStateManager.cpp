#include <Engine/GameStateManager.hpp>

std::shared_ptr<GameStateManager> GameStateManager::_mSelf = 0;
std::shared_ptr<GameStateManager> GameStateManager::GetInstance() {
	if (!_mSelf) return _mSelf = std::make_shared<GameStateManager>();
	return _mSelf;
}
void GameStateManager::PushState(std::unique_ptr<GameState> state) {
	if (!mStates.empty())
		mStates.top()->Exit();
	mStates.push(std::move(state));
	mStates.top()->Init();
}
void GameStateManager::PopState() {
	if (mStates.empty()) return;
	mStates.top()->Exit();
	mStates.pop();
	mStates.top()->Init();
}
void GameStateManager::Update(float dt) {
	if (mStates.empty()) return;
	mStates.top()->Update(dt);
}
void GameStateManager::Render(float dt) {
	if (mStates.empty()) return;
	mStates.top()->Render(dt);
}