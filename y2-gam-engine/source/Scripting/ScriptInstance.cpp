/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptInstance.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Dec 25, 2023

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
  ScriptInstance::ScriptInstance(ScriptClass scriptClass) : mInstance{ scriptClass.Instantiate() },
    mConstructor{ scriptClass.GetMethod(".ctor", 1) },
    mOnCreateMethod{ scriptClass.GetMethod("OnCreate", 0) },
    mOnUpdateMethod{ scriptClass.GetMethod("OnUpdate", 1) },
    mOnExitMethod{ scriptClass.GetMethod("OnExit", 0) },
    mScriptClass{ scriptClass } {

    gcHandle = mono_gchandle_new(mInstance, false);
  }
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
    //mScriptClass.CallThunkSingleArg(mInstance, mConstructor, entityHandle);
    gcHandle = mono_gchandle_new(mInstance, false);
  }

  /*  _________________________________________________________________________ */
  /*! CallOnCreate

  @return none.

  Calls the on create function from C#.
  */
  void ScriptInstance::CallOnCreate(Entity entityID) noexcept {
    void* param{ &entityID };
    mScriptClass.CallMethod(mInstance, mConstructor, &param);
    mScriptClass.CallMethod(mono_gchandle_get_target(gcHandle), mOnCreateMethod);
    //mScriptClass.CallThunkNoArg(mono_gchandle_get_target(gcHandle), mOnCreateMethod);
  }

  /*  _________________________________________________________________________ */
  /*! CallOnUpdate
  
  @param dt
  Delta time.

  @return none.

  Calls the on create update from C#. To be called every loop.
  */
  void ScriptInstance::CallOnUpdate(float dt) noexcept {
    void* dtParam{ &dt };
    mScriptClass.CallMethod(mono_gchandle_get_target(gcHandle), mOnUpdateMethod, &dtParam);
    //mScriptClass.CallThunkSingleArg(mono_gchandle_get_target(gcHandle), mOnUpdateMethod, dt);
  }

  /*  _________________________________________________________________________ */
  /*! CallOnExit

  @return none.

  Calls the on exit function from C#.
  */
  void ScriptInstance::CallOnExit() noexcept {
    mScriptClass.CallMethod(mono_gchandle_get_target(gcHandle), mOnExitMethod);
    //mScriptClass.CallThunkNoArg(mono_gchandle_get_target(gcHandle), mOnExitMethod);
    mono_gchandle_free(gcHandle);
  }

  /*  _________________________________________________________________________ */
  /*! GetFieldValueFromNameInternal

  @param fieldName
  The field name to get its value from.

  @return T&
  Template reference for the value from its field name.

  Internal function for using mono's field retrival function to get 
  the field value.
  */
  bool ScriptInstance::GetFieldValueFromNameInternal(std::string const& fieldName, void* buffer) {
    auto const& fields{ mScriptClass.GetFieldNameToTypeMap() };
    for (std::pair<std::string, Image::Field> val : fields) {
      if (val.first == fieldName) {
        Field const& field{ val.second };
        mono_field_get_value(mInstance, field.classField, buffer);
        return true;
      }
    }

    return false;
  }

  /*  _________________________________________________________________________ */
  /*! SetFieldValueWithNameInternal

  @param fieldName
  The field name to set the value of.

  @param val
  Value to set.

  @return bool
  Returns if the operation was successful or not.

  Sets the field value using mono method.
  */
  bool ScriptInstance::SetFieldValueWithNameInternal(std::string const& fieldName, void* value) {
    auto const& fields{ mScriptClass.GetFieldNameToTypeMap() };
    for (std::pair<std::string, Image::Field> val : fields) {
      if (val.first == fieldName) {
        Field const& field{ val.second };
        mono_field_set_value(mInstance, field.classField, value);
        return true;
      }
    }

    return false;
  }
}
