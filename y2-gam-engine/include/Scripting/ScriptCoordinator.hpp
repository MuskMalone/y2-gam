/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptCoordinator.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      Communicates with the C# scripts, allowing for internal calls
            in which the information from CPP code can be accessed in C#,
            and vice versa.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

namespace Image {
  class ScriptCoordinator {
  public:
    static void RegisterFunctions();
  };
}