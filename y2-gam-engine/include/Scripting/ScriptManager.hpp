/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptManager.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      Header file for the script manager. Highest level for scripts.
            It initalizes mono, loads assembly (.dll) files and creates
            mono classes for the respective objects.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Scripting/ScriptClass.hpp"
#include "Core/Coordinator.hpp"
#include "ScriptInstance.hpp"
#include "Components/Script.hpp"
#include "Scripting/ScriptCoordinator.hpp"

namespace Image {
  class ScriptManager {
  public:
    static void Init();
    static void Exit();
    static MonoAssembly* LoadCSharpAssembly(std::string const& assemblyFilePath);
    static void PrintMonoAssemblyTypes(MonoAssembly* assembly);
    static void PopulateEntityClassesFromAssembly(MonoAssembly* assembly);
    static void OnCreateEntity(Entity const& entity);
    static void OnUpdateEntity(Entity const& entity, float dt);

    static bool EntityClassExists(std::string const& className);

    // Getters
    static MonoDomain* GetAppDomain() { return sAppDomain; }
    static std::unordered_map<std::string, ScriptClass> const& GetEntityClasses() { return sEntityClasses; };
    static std::unordered_map<Entity, ScriptInstance> const& GetEntityInstances() { return sEntityInstances; };

  private:
    static char* LoadFile(std::string const& filePath, size_t& fileSize);
    static void InitMono();
    static void ExitMono();

  private:
    static MonoDomain* sRootDomain;
    static MonoDomain* sAppDomain;
    static std::unordered_map<std::string, ScriptClass> sEntityClasses;
    static std::unordered_map<Entity, ScriptInstance> sEntityInstances;
  };
}