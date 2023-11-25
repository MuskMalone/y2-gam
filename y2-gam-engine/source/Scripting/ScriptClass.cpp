/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptClass.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      The source file for script classes. Allows for calling the
            functions of specific script instances.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"

#include "Scripting/ScriptClass.hpp"
#include "Scripting/ScriptManager.hpp"

namespace Image {
  /*  _________________________________________________________________________ */
  /*! ScriptClass

  @param nsName
  The namespace the class is in.

  @param className
  The name of the class.

  @param monoClass
  The mono class.

  @return *this

  The non-default constructor for the script class.
  */
  ScriptClass::ScriptClass(std::string const& nsName, std::string const& className, MonoClass* monoClass) :
    mClassNamespace{ nsName }, mClassName{ className }, mMonoClass{ monoClass } { /* Empty By Design */ }

  /*  _________________________________________________________________________ */
  /*! Instantiate

  @return MonoObject*

  Creates a new mono object by allocating memory for the instanc of the class.
  */
  MonoObject* ScriptClass::Instantiate() const {
    // Allocate memory for an instance of the class
    MonoObject* classInstance = mono_object_new(ScriptManager::GetAppDomain(), mMonoClass);

    if (classInstance == nullptr) {
      //std::cout << "Mono Exception: Failed to create instance of class" << "\n";
#ifndef _INSTALLER
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Mono Exception: Failed to create instance of class", __FUNCTION__);
#endif
      std::exit(0);
    }

    return classInstance;
  }

  /*  _________________________________________________________________________ */
  /*! GetMethod

  @param name
  Name of the method.

  @param numParameters
  Number of parameters in the method.

  @return MonoMethod*

  Creates a mono method based on the name and number of parameters. Mono will
  look through the C# scripts for the respective method (or function).
  */
  MonoMethod* ScriptClass::GetMethod(std::string const& name, int numParameters) {
    MonoMethod* ret{ mono_class_get_method_from_name(mMonoClass, name.c_str(), numParameters) };

    if (ret == nullptr) {
#ifndef _INSTALLER
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Mono Exception: method does not exist!" + name + std::to_string(numParameters), __FUNCTION__);
#endif
      std::exit(0);
    }
    else {
#ifndef _INSTALLER
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Mono method " + name + " created!", __FUNCTION__);
#endif
    }

    return ret;
  }

  /*  _________________________________________________________________________ */
  /*! CallMethod

  @param instance
  The mono object instance.

  @param method
  The mono method.

  @param params
  The list of parameters that you want to pass into the function.

  @return MonoObject*

  Give a mono method, calls the function.
  */
  MonoObject* ScriptClass::CallMethod(MonoObject* instance, MonoMethod* method, void** params) {

    // FOR THE FUTURE
    // change to unmanaged thunks

    MonoObject* exception = nullptr;
    MonoError error;
    mono_error_init(&error);

    MonoObject* obj{ mono_runtime_invoke(method, instance, params, &exception) };

    //if (exception != nullptr) {
      //return obj;
    //}

    if (obj == nullptr) {
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Exception during method invocation in %s", __FUNCTION__);
    }

#ifndef _INSTALLER
     if (exception != nullptr) {
      MonoClass* exceptionClass = mono_object_get_class(exception);
      MonoMethod* toStringMethod = mono_class_get_method_from_name(exceptionClass, "ToString", 0);

        if (toStringMethod != nullptr) {
          void* toStringParams[] = { nullptr };
          MonoString* exceptionString = reinterpret_cast<MonoString*>(mono_runtime_invoke(toStringMethod, exception, toStringParams, nullptr));

          // Convert the MonoString to a C string
          const char* exceptionMessage = mono_string_to_utf8(exceptionString);

          // Handle the exception, log the details, etc.

          LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Exception occurred during method invocation: %s", exceptionMessage);

          // Release the memory used by the C string
          mono_free(const_cast<char*>(exceptionMessage));
        }
      }
#endif
      return obj;
  }
}