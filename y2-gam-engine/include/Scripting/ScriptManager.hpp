/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptManager.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Dec 24, 2023

\brief      Header file for the script manager. Highest level for scripts.
            It initalizes mono, loads assembly (.dll) files and creates
            mono classes for the respective objects.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Core/Coordinator.hpp"
#include "Components/Script.hpp"

#include "Scripting/ScriptClass.hpp"
#include "Scripting/ScriptInstance.hpp"
#include "Scripting/ScriptCoordinator.hpp"
#include "Scripting/ScriptFieldType.hpp"

namespace Image {
  class ScriptManager {
  public:
    static void Init();
    static void ExitMono();
    static MonoAssembly* LoadCSharpAssembly(std::string const& assemblyFilePath);
    static void PrintMonoAssemblyTypes(MonoAssembly* assembly);
    static void PopulateEntityClassesFromAssembly(MonoAssembly* assembly);
    static void OnCreateEntity(Entity const& entity);
    static void OnCreateEntityEvent(Event& event);
    static void LoadEntityLinkage(Entity entity, std::string tag);
    static void OnUpdateEntity(Entity const& entity, float dt);
    static void OnExitEntity(Entity const& entity);
    static void RemoveEntity(Entity const& entity);
    
    // Helpers
    static bool EntityClassExists(std::string const& className);
    static void PrintEntityInstances();

    static FieldType MonoToScriptType(MonoType* monoType);
    static std::string FieldTypeToString(FieldType fieldType);

    // Getters
    static MonoDomain* GetAppDomain() { return sAppDomain; }
    static std::unordered_map<std::string, ScriptClass> const& GetEntityClasses() { return sEntityClasses; };
    static std::map<Entity, ScriptInstance> const& GetEntityInstances() { return sEntityInstances; };
    static std::vector<const char *> const& GetAssignableScriptNames() { return sAssignableScriptNames; }
    static ScriptInstance& GetEntityScriptInstance(Entity const& entity);
    static ScriptInstance& GetTagToRawScriptInstance(std::string const& tag);
    static ScriptInstance& CreateScriptInstanceWithTag(std::string const& scriptName, std::string const& tag);
    static MonoObject* GetEntityMonoInstanceObject(Entity const& entity);

  private:
    static char* LoadFile(std::string const& filePath, size_t& fileSize);
    static void InitMono();
    static void Exit();

  private:
    static MonoDomain* sRootDomain;
    static MonoDomain* sAppDomain;
    static std::unordered_map<std::string, ScriptClass> sEntityClasses;
    static std::map<Entity, ScriptInstance> sEntityInstances;
    static std::map<std::string, ScriptInstance> sTagToRawInstances;
    static std::vector<const char*> sAssignableScriptNames;
    static std::map<std::string, Field> sScriptFieldTypes;
  };
}