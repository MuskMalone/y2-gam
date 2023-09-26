#include <Core/FrameRateController.hpp>
#include <chrono>
#include <exception>
#include <thread>
#include <iostream>
#include <Engine/StateManager.hpp>
std::shared_ptr<FrameRateController> FrameRateController::_mSelf = 0;
std::shared_ptr<FrameRateController> FrameRateController::GetInstance() {
	if (!_mSelf) return _mSelf = std::make_shared<FrameRateController>();
	return _mSelf;
}

void FrameRateController::Init(int fps, bool vsync) {


	mFps = static_cast<float>(fps);
	mTargetFps = mFps;
	mDeltaTime = 1.f / static_cast<float>(fps);
	mTargetDeltaTime = mDeltaTime;
	mVsync = vsync;
	mAccumulator = 0.f;
	glfwSwapInterval((vsync) ? 1 : 0);

}
void FrameRateController::StartFrameTime() {
	mStart = std::chrono::high_resolution_clock::now();
	mAccumulator += mDeltaTime;
}

void FrameRateController::AccumulateDt() {
	while (mAccumulator >= mTargetDeltaTime) {
		StateManager::GetInstance()->Update(mTargetDeltaTime);
		mAccumulator -= mTargetDeltaTime;
	}
}

float FrameRateController::EndFrameTime() {
	mDeltaTime = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - mStart).count();
	if (mDeltaTime < mTargetDeltaTime) {
		std::this_thread::sleep_for(std::chrono::duration<double>(mTargetDeltaTime - mDeltaTime));
		mDeltaTime = mTargetDeltaTime;
	}
	++mFpsCounter;

	//gets the fps
	if (!(mFpsCounter % static_cast<int>(mTargetFps))) {
		mFps = 1.f / mDeltaTime;
	}

	return mDeltaTime;
}

void FrameRateController::StartSubFrameTime() {

}
float FrameRateController::EndSubFrameTime() {
	return 0.f;
}