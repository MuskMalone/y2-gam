/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptMessagingWrapper.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Dec 22, 2023

\brief      Wrapper functions for the messaging system to be used in C#.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

namespace Image {
  class ScriptMessagingWrapper {
  public:
    static void RegisterFunctions();
  };
}