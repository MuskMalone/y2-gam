/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptClass.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      The header file for script classes. Allows for calling the
            functions of specific script instances.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

// For unmanaged thunks
typedef void (*MonoFunctionThunkNone)(MonoObject*, MonoException**);
typedef void (*MonoFunctionThunkSingle)(MonoObject*, float, MonoException**);

namespace Image {
  class ScriptClass {
  public:
    ScriptClass() = default;
    ScriptClass(std::string const& nsName, std::string const& className, MonoClass* monoClass);

    MonoObject* Instantiate() const;
    MonoMethod* GetMethod(std::string const& name, int numParameters);
    MonoObject* CallMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
    void CallThunkSingleArg(MonoObject* instance, MonoMethod* method, float arg);
    void CallThunkNoArg(MonoObject* instance, MonoMethod* method);

    std::string GetNamespace() const { return mClassNamespace; }
    std::string GetClassName() const { return mClassName; }

  private:
    std::string mClassNamespace{};
    std::string mClassName{};
    MonoClass* mMonoClass{ nullptr };
  };
}