/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptInstance.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      The source file for specific script instances. The lowest level.
            It facilitates calling the specific funcitons in the instance.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "Scripting/ScriptInstance.hpp"

namespace Image {
  /*  _________________________________________________________________________ */
  /*! ScriptInstance

  @param scriptClass
  The script class.

  @param entityHandle
  The entity to create a script instance for.

  @return *this

  The non-default constructor for the script instance class.
  */
  ScriptInstance::ScriptInstance(ScriptClass scriptClass, Entity entityHandle) : 
    mInstance{ scriptClass.Instantiate() },
    mConstructor{ scriptClass.GetMethod(".ctor", 1) },
    mOnCreateMethod{ scriptClass.GetMethod("OnCreate", 0) },
    mOnUpdateMethod{ scriptClass.GetMethod("OnUpdate", 1) },
    mOnExitMethod{ scriptClass.GetMethod("OnExit", 0) },
    mScriptClass{ scriptClass } {

    // Call the non-default, single parameter constructor from C#
    // The parameter is the entity id
    void* param{ &entityHandle };
    mScriptClass.CallMethod(mInstance, mConstructor, &param);
  }

  /*  _________________________________________________________________________ */
  /*! CallOnCreate

  @return none.

  Calls the on create function from C#.
  */
  void ScriptInstance::CallOnCreate() {
    if (mInstance != nullptr && mOnCreateMethod != nullptr)
      mScriptClass.CallMethod(mInstance, mOnCreateMethod);
  }

  /*  _________________________________________________________________________ */
  /*! CallOnUpdate
  
  @param dt
  Delta time.

  @return none.

  Calls the on create update from C#. To be called every loop.
  */
  void ScriptInstance::CallOnUpdate(float dt) {
    void* dtParam{ &dt };
    if (mInstance != nullptr && mOnUpdateMethod != nullptr && dtParam != nullptr)
      mScriptClass.CallMethod(mInstance, mOnUpdateMethod, &dtParam);
  }

  /*  _________________________________________________________________________ */
  /*! CallOnExit

  @return none.

  Calls the on exit function from C#.
  */
  void ScriptInstance::CallOnExit() {
    if (mInstance != nullptr && mOnExitMethod != nullptr)
      mScriptClass.CallMethod(mInstance, mOnExitMethod);
  }
}
