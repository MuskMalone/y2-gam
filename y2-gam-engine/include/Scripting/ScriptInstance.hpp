/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptInstance.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Dec 25, 2023

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
    ScriptInstance(ScriptClass scriptClass);
    ScriptInstance(ScriptClass scriptClass, Entity entityHandle);

    void CallOnCreate(Entity entityID) noexcept;
    void CallOnUpdate(float dt) noexcept;
    void CallOnExit() noexcept;

    // Getters
    inline ScriptClass& GetScriptClass() { return mScriptClass; }

    /*  _________________________________________________________________________ */
    /*! GetFieldValueFromName

    @param fieldName
    The field name to get its value from.

    @return T
    Template reference for the value from its field name.

    Gets the field value from its field name of a specific script instance.
    */
    template <typename T>
    inline T GetFieldValueFromName(std::string const& fieldName) {
      bool success{ GetFieldValueFromNameInternal(fieldName, sFieldValueBuffer) };

      if (!success) {
        return T();
      }

      return *(T*)sFieldValueBuffer;
    }

    // Setters
    /*  _________________________________________________________________________ */
    /*! SetFieldValueWithName

    @param fieldName
    The field name to set the value of.

    @param val
    Value to set.

    @return none.

    Sets the field value from its field name of a specific script instance.
    */
    template <typename T>
    inline void SetFieldValueWithName(std::string const& fieldName, const T& val) {
      SetFieldValueWithNameInternal(fieldName, (void*)&val);
    }

    // For Strings
    inline MonoString* GetFieldValueFromStringName(std::string const& fieldName) {
      auto const& fields{ mScriptClass.GetFieldNameToTypeMap() };     
      for (std::pair<std::string, Image::Field> val : fields) {
        if (val.first == fieldName) {
          Field const& field{ val.second };
          MonoString* monoStr = nullptr;
          mono_field_get_value(mInstance, field.classField, &monoStr);
          return monoStr;
        }
      }

      return nullptr;
    }

    inline void SetFieldValueWithStringName(std::string const& fieldName, MonoString* val) {
      SetFieldValueWithNameInternal(fieldName, reinterpret_cast<void*>(val));
    }

    MonoObject* GetMonoInstanceObject() { return mInstance; }

  private:
    bool GetFieldValueFromNameInternal(std::string const& fieldName, void* buffer);
    bool SetFieldValueWithNameInternal(std::string const& fieldName, void* value);

  private:
    MonoObject* mInstance{ nullptr };
    MonoMethod* mConstructor{ nullptr };
    MonoMethod* mOnCreateMethod{ nullptr };
    MonoMethod* mOnUpdateMethod{ nullptr };
    MonoMethod* mOnExitMethod{ nullptr };
    ScriptClass mScriptClass{};
    unsigned int gcHandle{};

    inline static char sFieldValueBuffer[8];
  };
}