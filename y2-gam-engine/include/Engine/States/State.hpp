#pragma once
class State {
public:
	virtual void Init() = 0;
	virtual void Exit() = 0;

	virtual void Update(float dt) = 0;
	virtual void Render(float dt) = 0;

};