#pragma once
#include <Core/Globals.hpp>
#include <Core/Types.hpp>
#include <memory>
#include <queue>
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class FrameRateController {
public:
	static std::shared_ptr<FrameRateController> GetInstance();

	void Init(int fps, bool vsync);
	void StartFrameTime();
	float EndFrameTime();

	void StartSubFrameTime();
	float EndSubFrameTime();

	float GetFps() { return mFps;  }
	float GetDeltaTime() { return mDeltaTime; }
	
private:
	static std::shared_ptr<FrameRateController> _mSelf;
	std::chrono::steady_clock::time_point mStart{};
	bool mVsync{};
	float mTargetDeltaTime{};
	float mDeltaTime{};
	float mFps{};
	float mTargetFps{};
	size_t fpsCounter{ 0 };
	std::queue<std::chrono::steady_clock::time_point> subDelta{};
};