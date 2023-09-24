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

#include "../include/pch.h"
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
      std::cout << "Mono Exception: Failed to create instance of class" << "\n";
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
      std::cout << "Mono Exception: method does not exist!" << "\n";
      std::cout << name << "\n";
      std::cout << numParameters << "\n";
      std::exit(0);
    }
    else {
      std::cout << "Mono method " << name << " created!" << "\n";
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
    return mono_runtime_invoke(method, instance, params, nullptr);
  }
}