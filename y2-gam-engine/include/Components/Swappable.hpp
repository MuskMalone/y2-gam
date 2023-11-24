/******************************************************************************/
/*!
\par        Image Engine
\file       Swappable.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 24, 2023

\brief      Header file for Tag component

            This file sets up a TagComponent to be used for the ECS system.
            It allows entities to be tagged with a name.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <string>
#include <rapidjson/document.h>
#include <Core/Serialization/SerializationManager.hpp>

struct Swappable {
  Swappable() = default;
  Swappable([[maybe_unused]]rapidjson::Value const& obj) {

  }

  /*  _________________________________________________________________________ */
  /*! Serialize

  @param none.

  @return bool
  returns true

  This function is inserts values into the serializer
  */
  bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
    return true;
  }
};