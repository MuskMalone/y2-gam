/******************************************************************************/
/*!
\par        Image Engine
\file       StateManager.cpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		state layering

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include <Engine/StateManager.hpp>

std::shared_ptr<StateManager> StateManager::_mSelf = 0;
std::shared_ptr<StateManager> StateManager::GetInstance() {
	if (!_mSelf) return _mSelf = std::make_shared<StateManager>();
	return _mSelf;
}
/*  _________________________________________________________________________ */
/*! PopState

@return none.

Pops the top state from the state stack. If there are remaining states, initializes the new top state.
*/

void StateManager::PopState() {
	if (mStates.empty()) return;
	mStates.top()->Exit();
	mStates.pop();
	if (mStates.empty()) return;
	mStates.top()->Init();
}
/*  _________________________________________________________________________ */
/*! Update

@param dt The delta time since the last frame.

@return none.

Updates the top state in the state stack with the given delta time.
*/

void StateManager::Update(float dt) {
	if (mStates.empty()) return;
	mStates.top()->Update(dt);
}
/*  _________________________________________________________________________ */
/*! Render

@param dt The delta time since the last frame.

@return none.

Renders the top state in the state stack with the given delta time.
*/

void StateManager::Render(float dt) {
	if (mStates.empty()) return;
	mStates.top()->Render(dt);
}
/*  _________________________________________________________________________ */
/*! Clear

@return none.

Clears all states from the state stack, calling the Exit method for each state.
*/

void StateManager::Clear() {
	while (!mStates.empty()) {
		mStates.top()->Exit();
		mStates.pop();
	}
}