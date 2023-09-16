#include "Systems/AnimationSystem.hpp"
#include "Core/Coordinator.hpp"
#include "Components/Animation.hpp"
#include "Components/Sprite.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

void AnimationSystem::Init() {
	::gCoordinator = Coordinator::GetInstance();

	//-------------TEMPORARY-------------------------------------------------------------/
	mSpriteSheet.reset(new Texture{ "../Textures/Attack_1.png" });
	for (float i{}; i < 7; ++i)
		mSpriteList.push_back(SubTexture::Create(mSpriteSheet, { i, 0 }, { 128, 128 }));

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

		AnimationFrame currFrame { frameList[frameIdx] };

		currFrame.elapsedTime += dt;
		sprite.texture = mSpriteList[currFrame.spriteIdx];

		if (currFrame.elapsedTime >= animation.speed) {
			++frameIdx;
			currFrame.elapsedTime = 0.f;
		}
	}
}
