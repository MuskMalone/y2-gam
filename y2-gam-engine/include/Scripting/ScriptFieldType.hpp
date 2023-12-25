/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptFieldType.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Dec 24, 2023

\brief      The header file for the script field types to be supported for
            exposure to IMGUI. When these field types are public in the scripts,
            this enum class facilitates what can and cannot be displayed.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

namespace Image {
  enum class FieldType {
    None = 0,
    Float,
    Double,
    Bool,
    Char,
    Byte,
    Short,
    Int,
    Long,
    UByte,
    UShort,
    UInt,
    ULong,
    Vector2,
    Vector3,
    Vector4,
    Entity
  };

  struct Field {
    FieldType fieldType;
    MonoClassField* classField;
  };
}