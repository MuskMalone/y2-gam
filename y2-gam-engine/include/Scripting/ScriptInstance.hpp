/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptInstance.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      The header file for specific script instances. The lowest level.
            It facilitates calling the specific funcitons in the instance.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "ScriptClass.hpp"

namespace Image {
  class ScriptInstance {
  public:
    ScriptInstance() = default;
    ScriptInstance(ScriptClass scriptClass, Entity entityHandle);

    void CallOnCreate();
    void CallOnUpdate(float dt);
    void CallOnExit();

  private:
    MonoObject* mInstance{ nullptr };
    MonoMethod* mConstructor{ nullptr };
    MonoMethod* mOnCreateMethod{ nullptr };
    MonoMethod* mOnUpdateMethod{ nullptr };
    MonoMethod* mOnExitMethod{ nullptr };
    ScriptClass mScriptClass{};
    unsigned int gcHandle{};
  };
}