#include "Systems/AnimationSystem.hpp"
#include "Core/Coordinator.hpp"
#include "Components/Animation.hpp"
#include "Components/Sprite.hpp"
#include "Systems/InputSystem.hpp"
#include "GLFW/glfw3.h"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

void AnimationSystem::Init() {
	::gCoordinator = Coordinator::GetInstance();

	//-------------TEMPORARY-------------------------------------------------------------/
	mIdle.reset(new Texture{ "../Textures/Idle.png" });
	mRun.reset(new Texture{ "../Textures/Run.png" });
	mAttack.reset(new Texture{ "../Textures/Attack_1.png" });
	for (float i{}; i < 8; ++i)
		mSpriteList.push_back(SubTexture::Create(mIdle, { i, 0 }, { 128, 128 }));

	for(float i{8}; i < 16; ++i)
		mSpriteList.push_back(SubTexture::Create(mRun, {i, 0}, {128, 128}));

	for(float i{16}; i <23; ++i)
		mSpriteList.push_back(SubTexture::Create(mAttack, { i, 0 }, { 128, 128 }));

	//------------------------------------------------------------------------------------/

}

void AnimationSystem::Update(float dt) {
	for (auto const& entity : mEntities)
	{
		auto& sprite = gCoordinator->GetComponent<Sprite>(entity);
		auto& animation = gCoordinator->GetComponent<Animation>(entity);

		size_t& frameIdx { animation.currFrame };
		std::vector<AnimationFrame>& frameList{ animation.stateMap[animation.currState] };

		if (frameIdx >= frameList.size())
			frameIdx = 0;

		AnimationFrame& currFrame { frameList[frameIdx] };

		currFrame.elapsedTime += dt;
		sprite.texture = mSpriteList[currFrame.spriteIdx];

		if (currFrame.elapsedTime >= animation.speed) {

			++frameIdx;
			currFrame.elapsedTime = 0.f;
		}

		auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
		if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_O)) {
			switch (animation.currState) {
			case ANIM_STATE::IDLE:
				animation.currState = ANIM_STATE::RUN;
				break;
			case ANIM_STATE::RUN: 
				animation.currState = ANIM_STATE::ATTACK;
				break;
			case ANIM_STATE::ATTACK:
				animation.currState = ANIM_STATE::IDLE;
				break;
			}
		}


	}
}
