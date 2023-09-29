#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       StateManager.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		state layering 

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <stack>
#include <Engine/States/State.hpp>
#include <memory>

class StateManager {
public:
	static std::shared_ptr<StateManager> GetInstance();
	template <typename _state>
	void PushState() {
		if (!mStates.empty())
			mStates.top()->Exit();
		mStates.push(std::move(std::make_unique<_state>()));
		//mStates.push(std::move(std::make_unique<State>()));
		mStates.top()->Init();
	}
	void PopState();
	void Update(float dt);
	void Render(float dt);
	void Clear();

private:
	std::stack<std::unique_ptr<State>> mStates;
	static std::shared_ptr<StateManager> _mSelf;
};