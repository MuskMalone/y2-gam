/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptMessagingWrapper.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Dec 22, 2023

\brief      Wrapper functions for the messaging system to be used in C#.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"

#include "Scripting/ScriptCoordinator.hpp"
#include "Scripting/ScriptManager.hpp"
#include "Scripting/ScriptMessagingWrapper.hpp"

#include "Core/EventManager.hpp"


namespace Image {

#define IMAGE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Image.InternalCalls::" #Name, Name)

  static void AddListener() {

  }

  static void SendEvent(EventId eventId) {

  }

  template<typename T>
  void SetParam(EventId id, T value) {

  }

  void ScriptMessagingWrapper::RegisterFunctions() {
    IMAGE_ADD_INTERNAL_CALL(AddListener);
  }
}

