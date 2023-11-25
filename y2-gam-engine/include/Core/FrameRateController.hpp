/******************************************************************************/
/*!
\par        Frame rate controller
\file       FrameRateController.hpp

\author     Ng Wen Wen (wenwen.ng@digipen.edu)
\date       Sep 4, 2023

\brief      Implementation file for frame rate controller

			This source file manages frame rates, delta times, and 
			profiling in an application or game.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
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
	void ScaleDeltaTime(float factor);
	inline float GetFps() { return mFps;  }
	inline float GetDeltaTime() { return mDeltaTime; }
	inline float GetTargetDT() { return mTargetDeltaTime; }

	static bool GetIsGameSlow() noexcept { return isGameSlow; }
	static void SetIsGameSlow(bool flag) noexcept { isGameSlow = flag; }
	
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
	static bool isGameSlow;
	float slowFactor;
};