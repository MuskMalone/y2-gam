/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptManager.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      Source file for the script manager. Highest level for scripts.
            It initalizes mono, loads assembly (.dll) files and creates
            mono classes for the respective objects.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.h"
#include "Scripting/ScriptManager.hpp"
#include "Logging/LoggingSystem.hpp";
#include <Logging/backward.hpp>

namespace {
  std::shared_ptr<Coordinator> gCoordinator;
}

namespace Image {

  MonoDomain* ScriptManager::sRootDomain{ nullptr };
  MonoDomain* ScriptManager::sAppDomain{ nullptr };
  std::unordered_map<std::string, ScriptClass> ScriptManager::sEntityClasses{};
  std::unordered_map<Entity, ScriptInstance> ScriptManager::sEntityInstances{};

  /*  _________________________________________________________________________ */
  /*! Init

  @return none.

  The main init. Calls the respective script related functions.
  */
  void ScriptManager::Init() {
    InitMono();
    ScriptCoordinator::RegisterFunctions();
  }

  /*  _________________________________________________________________________ */
  /*! Exit

  @return none.

  The main exit.
  */
  void ScriptManager::Exit() {
    // Apparently mono cleans up after itself, empty for now
  }

  /*  _________________________________________________________________________ */
  /*! InitMono

  @return none.

  Initializes Mono.
  */
  void ScriptManager::InitMono() {
    // Looks for mscorlib.dll (Using .NET 4.5), required for C#
    mono_set_assemblies_path("../assets/scripts");

    // Mono auto detects the runtime version based on first assembly loaded
    MonoDomain* rootDomain{ mono_jit_init("ScriptRuntime") };

    if (rootDomain == nullptr) {
      //std::cout << "Root Domain Initialization Failed!" << "\n";
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Root Domain Initialization Failed!", __FUNCTION__);
      return;
    }
    sRootDomain = rootDomain;

    char appDomainName[] = "MyAppDomain";
    // An app domain is a C# language feature
    MonoDomain* appDomain = mono_domain_create_appdomain(appDomainName, nullptr);
    if (appDomain == nullptr) {
      //std::cout << "App Domain Initialization Failed!" << "\n";
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "App Domain Initialization Failed!", __FUNCTION__);
      return;
    }
    sAppDomain = appDomain;

    mono_domain_set(sAppDomain, true);
  }

  void ScriptManager::ExitMono() {

  }

  /*  _________________________________________________________________________ */
  /*! LoadFile

  @param filePath

  @param fileSize
  Size of the final file.

  @return char*
  Pointer to the file loaded in memory.

  A generic function for loading the provided file from the file path into memory, 
  as a series of chars AKA single bytes. Remember to free memory after use!
  */
  char* ScriptManager::LoadFile(std::string const& filePath, size_t& fileSize) {
    std::fstream ifs(filePath, std::ios::in | std::ios::binary);
    if (!ifs) {
      //std::cout << "File " << filePath << " could not be opened!" << "\n";
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "File " + filePath + " could not be opened", __FUNCTION__);
      return nullptr;
    }

    // get size of file
    ifs.seekg(0, ifs.end);
    fileSize = ifs.tellg();
    ifs.seekg(0, ifs.beg);

    if (fileSize == 0) {
      //std::cout << "File being read is empty!" << "\n";
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "File being read is empty!", __FUNCTION__);
      return nullptr;
    }

    char* fileBuffer = new char[fileSize];
    ifs.read(fileBuffer, fileSize);
    if (!ifs) {
      //std::cout << "File reading error!" << "\n";
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "File reading error!", __FUNCTION__);
      return nullptr;
    }
    ifs.close();

    return fileBuffer;
  }

  /*  _________________________________________________________________________ */
  /*! LoadCSharpAssembly

  @param assemblyPath
  Path to the C# assembly (DLL) file.

  @return MonoAssembly*
  Pointer to the created MonoAssembly.

  Given a filepath to an assembly (usually a DLL file), loads it in as a
  MonoAssembly.
  */
  MonoAssembly* ScriptManager::LoadCSharpAssembly(std::string const& assemblyFilePath) {
    size_t fileSize{};
    char* fileData{ LoadFile(assemblyFilePath, fileSize) };

    // Enum for the MonoImage opening status
    MonoImageOpenStatus status{};

    // MonoImage contains compiled binary file that contains executable code
    MonoImage* monoImage{ mono_image_open_from_data_full(fileData, static_cast<uint32_t>(fileSize), 1, &status, 0) };

    if (status != MonoImageOpenStatus::MONO_IMAGE_OK) {
      std::cout << mono_image_strerror(status) << "\n";
      return nullptr;
    }

    MonoAssembly* monoAssembly{ mono_assembly_load_from_full(monoImage, 
      assemblyFilePath.c_str(), &status, 0) };

    if (status != MonoImageOpenStatus::MONO_IMAGE_OK) {
      std::cout << mono_image_strerror(status) << "\n";
      return nullptr;
    }

    mono_image_close(monoImage);
    delete[] fileData;

#ifdef _DEBUG
    PrintMonoAssemblyTypes(monoAssembly);
#endif

    return monoAssembly;
  }

  /*  _________________________________________________________________________ */
  /*! PrintMonoAssemblyTypes

  @param assembly
  Pointer to the mono assembly.

  @return none.

  Prints all the namespace and class names from the loaded assembly. The first
  one will always be blank as it is mono itself!
  */
  void ScriptManager::PrintMonoAssemblyTypes(MonoAssembly* assembly) {
    MonoImage* image{ mono_assembly_get_image(assembly) };
    const MonoTableInfo* typeDefinitionsTable{ mono_image_get_table_info(image, MONO_TABLE_TYPEDEF) };
    int32_t numTypes{ mono_table_info_get_rows(typeDefinitionsTable) };

    for (int i{}; i < static_cast<int>(numTypes); i++) {
      uint32_t cols[MONO_TYPEDEF_SIZE];
      mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

      const char* nsName{ mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]) };
      const char* className{ mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]) };

      //std::cout << "Namespace: " << nsName << "\n";
      std::string str(nsName);
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Namespace: " + str, __FUNCTION__);
      //std::cout << "Class Name: " << className << "\n";
      std::string str1(className);
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Class Name: " + str1, __FUNCTION__);
    }
  }

  /*  _________________________________________________________________________ */
  /*! PopulateEntityClassesFromAssembly

  @param assembly
  Pointer to the mono assembly.

  @return none.

  Populates the unordered map for its name to its MonoClass.
  */
  void ScriptManager::PopulateEntityClassesFromAssembly(MonoAssembly* assembly) {
    MonoImage* image{ mono_assembly_get_image(assembly) };
    const MonoTableInfo* typeDefinitionsTable{ mono_image_get_table_info(image, MONO_TABLE_TYPEDEF) };
    int32_t numTypes{ mono_table_info_get_rows(typeDefinitionsTable) };

    for (int i{}; i < static_cast<int>(numTypes); i++) {
      uint32_t cols[MONO_TYPEDEF_SIZE];
      mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

      const char* nsName{ mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]) };
      const char* className{ mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]) };

      std::string nameOfNamespace{ nsName };
      std::string nameOfClass{ className };
      std::string finalName{};

      // Account for some classes not having a namespace in the script
      if (nameOfNamespace.empty()) {
        finalName = nameOfClass;
      }
      else {
        finalName = nameOfNamespace + nameOfClass;
      }

      MonoClass* entityClass{ mono_class_from_name(image, "Image", "Entity") };
      MonoClass* monoClass{ mono_class_from_name(image, nsName, className) };
      bool isEntity{ static_cast<bool>(mono_class_is_subclass_of(monoClass, entityClass, false)) };

      if (monoClass == entityClass) {
        continue;
      }

      ScriptClass sc{ nsName, className, monoClass };

      if (isEntity) {
        sEntityClasses[finalName] = sc;
      }
    }

#ifdef _DEBUG
      for (auto const& pair : sEntityClasses) {
        ScriptClass const& value = pair.second;
        //std::cout << "Key: " << pair.first << ", Namespace: " << value.GetNamespace() 
        //  << ", Class: " << value.GetClassName() << "\n";
      }
#endif
  }

  /*  _________________________________________________________________________ */
  /*! OnCreateEntity

  @param entity
  The entity handle for the created entity with a script component.

  @return none.

  Function to be called after creating an entity with script component.
  Creates a script instance and maps it to the entity ID, then calls its
  own create function.
  */
  void ScriptManager::OnCreateEntity(Entity const& entity) {
    ::gCoordinator = Coordinator::GetInstance();
    auto const& scriptComp{ gCoordinator->GetComponent<Script>(entity) };
    if (EntityClassExists(scriptComp.name)) {
      ScriptInstance si{ sEntityClasses[scriptComp.name], entity };
      sEntityInstances[entity] = si;
      si.CallOnCreate();

      std::cout << "Entity in the ECS with script component created!" << "\n";
    }
    else {
      std::cout << "Entity Script does not exist!" << "\n";
    }
  }

  /*  _________________________________________________________________________ */
  /*! OnUpdateEntity

  @param entity
  The entity handle for the created entity with a script component.

  @param dt
  Delta time.

  @return none.

  This function is called on update loop for the entity.
  */
  void ScriptManager::OnUpdateEntity(Entity const& entity, float dt) {
    sEntityInstances[entity].CallOnUpdate(dt);
  }

  /*  _________________________________________________________________________ */
  /*! EntityClassExists

  @param className
  The FULL name of the class (i.e. SandboxPlayer).

  @return bool
  True if exists, false if not.

  Returns if the entity class exists or not.
  */
  bool ScriptManager::EntityClassExists(std::string const& className) {
    return (sEntityClasses.find(className) != sEntityClasses.end()) ? true : false;
  }
}