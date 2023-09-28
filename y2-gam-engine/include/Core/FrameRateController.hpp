#pragma once
#include <Core/Globals.hpp>
#include <Core/Types.hpp>
#include <memory>
#include <queue>
#include <chrono>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class FrameRateController {
public:
	static std::shared_ptr<FrameRateController> GetInstance();

	void Init(int fps, bool vsync);
	void StartFrameTime();
	float EndFrameTime();

	void AccumulateDt();

	void StartSubFrameTime();
	float EndSubFrameTime(size_t key);
	float GetProfilerValue(size_t key);

	inline float GetFps() { return mFps;  }
	inline float GetDeltaTime() { return mDeltaTime; }
	inline float GetTargetDT() { return mTargetDeltaTime; }
	
private:
	static std::shared_ptr<FrameRateController> _mSelf;
	std::chrono::steady_clock::time_point mStart{};
	bool mVsync{};
	float mTargetDeltaTime{};
	float mDeltaTime{};
	float mFps{};
	float mTargetFps{};
	size_t mFpsCounter{ 0 };
	float mAccumulator{};
	std::map<size_t, float> mProfiler{};
	std::queue<std::chrono::steady_clock::time_point> mSubDelta{};
};