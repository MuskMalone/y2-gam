/******************************************************************************/
/*!
\par        Frame rate controller
\file       FrameRateController.cpp

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

#include "../include/pch.hpp"
#include <Core/FrameRateController.hpp>
#include <Engine/StateManager.hpp>

bool FrameRateController::isGameSlow;

std::shared_ptr<FrameRateController> FrameRateController::_mSelf = 0;
std::shared_ptr<FrameRateController> FrameRateController::GetInstance() {
	if (!_mSelf) return _mSelf = std::make_shared<FrameRateController>();
	return _mSelf;
}
/*  _________________________________________________________________________ */
/*! Init

@param fps The target frames per second.
@param vsync Flag to determine if vertical synchronization is enabled.

@return none.

Initializes the frame rate controller with the specified FPS and VSync settings.
*/
	
void FrameRateController::Init(int fps, bool vsync) {
	mFps = static_cast<float>(fps);
	mTargetFps = mFps;
	mDeltaTime = 1.f / static_cast<float>(fps);
	mTargetDeltaTime = mDeltaTime;
	mVsync = vsync;
	mAccumulator = 0.f;
	slowFactor = 0.f;
	isGameSlow = false;
	glfwSwapInterval((vsync) ? 1 : 0);

}
/*  _________________________________________________________________________ */
/*! StartFrameTime

@return none.

Starts the frame time measurement and prepares the controller for the new frame.
*/

void FrameRateController::StartFrameTime() {
	//clear subdelta stack
	while (!mSubDelta.empty()) mSubDelta.pop();
	//clear profiler map
	for (auto& pair : mProfiler) pair.second = 0.0f;
	mStart = std::chrono::high_resolution_clock::now();
	mAccumulator += mDeltaTime;
}
/*  _________________________________________________________________________ */
/*! AccumulateDt

@return none.

Accumulates the delta time and updates the state manager accordingly.
*/

void FrameRateController::AccumulateDt() {
	while (mAccumulator >= mTargetDeltaTime) {
		StateManager::GetInstance()->Update(mTargetDeltaTime);
		mAccumulator -= mTargetDeltaTime;
	}
}
/*  _________________________________________________________________________ */
/*! EndFrameTime

@return The delta time for the frame.

Ends the frame time measurement and adjusts the frame rate if necessary.
*/

float FrameRateController::EndFrameTime() {
	mDeltaTime = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - mStart).count();
	//if (mDeltaTime < mTargetDeltaTime) {
	//	std::this_thread::sleep_for(std::chrono::duration<double>(mTargetDeltaTime - mDeltaTime));
	//	mDeltaTime = mTargetDeltaTime;
	//}
	++mFpsCounter;
	static std::deque<float> fpsQueue;
	int targetFpsInt = static_cast<int>(mTargetFps);
	//gets the fps
	fpsQueue.push_back(1.f / mDeltaTime);
	if (fpsQueue.size() > targetFpsInt) {
		fpsQueue.pop_front();
	}
	mFps = { std::accumulate(fpsQueue.begin(), fpsQueue.end(), 0.f) / mTargetFps};

	//std::cout << mDeltaTime << " "<<mFps << std::endl;
	if (isGameSlow) {
		return mDeltaTime * slowFactor;
	}
	else {
		return mDeltaTime;
	}
}
/*  _________________________________________________________________________ */
/*! StartSubFrameTime

@return none.

Starts the sub-frame time measurement.
*/

void FrameRateController::StartSubFrameTime() {
	mSubDelta.emplace(std::chrono::high_resolution_clock::now());
}
/*  _________________________________________________________________________ */
/*! EndSubFrameTime

@param key The key associated with the sub-frame time measurement.

@return The delta time for the sub-frame.

Ends the sub-frame time measurement and updates the profiler with the measured time.
*/

float FrameRateController::EndSubFrameTime(size_t key) {
	float deltaTime{ std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - mSubDelta.front()).count() };
	mSubDelta.pop();

	mProfiler[key] += deltaTime;
	return mProfiler[key];
}
/*  _________________________________________________________________________ */
/*! GetProfilerValue

@param key The key associated with the sub-frame time measurement.

@return The profiler value for the specified key.

Retrieves the profiler value for the specified key, normalized by the frame's delta time.
*/

float FrameRateController::GetProfilerValue(size_t key) {
	if (mProfiler.find(key) == mProfiler.end()) return 0.0f;
	return mProfiler[key] / mDeltaTime;
}

void FrameRateController::ScaleDeltaTime(float factor){
	isGameSlow = !isGameSlow;
	if (isGameSlow) {
		slowFactor = factor;
	}
	else {
		slowFactor = 0.f;
	}
}

