#pragma once
#include "Core/System.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/SubTexture.hpp"
#include <memory>
#include <vector>

class Event;


class AnimationSystem : public System
{
public:
	void Init();

	void Update(float dt);

private:
	//TODO REMOVE ------------TEMP------------------
	std::shared_ptr<Texture> mIdle;
	std::shared_ptr<Texture> mRun;
	std::shared_ptr<Texture> mAttack;
	std::vector<std::shared_ptr<SubTexture>> mSpriteList;
};
