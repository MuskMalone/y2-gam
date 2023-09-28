/******************************************************************************/
/*!
\par        Image Engine
\file       AnimationSystem.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 12, 2023

\brief      Header for the AnimationSystem class.

			Manages and updates entity animations, interfacing with Texture and
			SubTexture classes for animation frames.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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
