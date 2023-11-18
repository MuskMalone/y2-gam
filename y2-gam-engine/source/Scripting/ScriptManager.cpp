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

#include "../include/pch.hpp"
#include "Scripting/ScriptManager.hpp"

namespace {
  std::shared_ptr<Coordinator> gCoordinator;
}

namespace Image {

  MonoDomain* ScriptManager::sRootDomain{ nullptr };
  MonoDomain* ScriptManager::sAppDomain{ nullptr };
  std::unordered_map<std::string, ScriptClass> ScriptManager::sEntityClasses{};
  std::unordered_map<Entity, ScriptInstance> ScriptManager::sEntityInstances{};
  std::vector<const char*> ScriptManager::sAssignableScriptNames{};

  /*  _________________________________________________________________________ */
  /*! Init

  @return none.

  The main init. Calls the respective script related functions.
  */
  void ScriptManager::Init() {
    InitMono();
    ScriptCoordinator::RegisterFunctions();
    MonoAssembly* ma{ Image::ScriptManager::LoadCSharpAssembly("../assets/scripts/y2-gam-script.dll") };
    Image::ScriptManager::PopulateEntityClassesFromAssembly(ma);
  }

  /*  _________________________________________________________________________ */
  /*! Exit

  @return none.

  The main exit.
  */
  void ScriptManager::Exit() {
    // Apparently mono cleans up after itself, empty for now

    mono_domain_set(mono_get_root_domain(), false);

    mono_domain_unload(sAppDomain);
    sAppDomain = nullptr;

    mono_jit_cleanup(sRootDomain);
    sRootDomain = nullptr;
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
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Root Domain Initialization Failed!", __FUNCTION__);
      return;
    }
    sRootDomain = rootDomain;

    char appDomainName[] = "MyAppDomain";
    // An app domain is a C# language feature
    MonoDomain* appDomain = mono_domain_create_appdomain(appDomainName, nullptr);
    if (appDomain == nullptr) {
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "App Domain Initialization Failed!", __FUNCTION__);
      return;
    }
    sAppDomain = appDomain;

    mono_domain_set(sAppDomain, true);
    Coordinator::GetInstance()->AddEventListener(FUNCTION_LISTENER(Events::System::ENTITY, ScriptManager::OnCreateEntityEvent));
  }

  void ScriptManager::ExitMono() {
    for (const char* str : sAssignableScriptNames) {
      free((void*)str); // Free the duplicated strings
    }
    sAssignableScriptNames.clear(); // Clear the vector

    Exit();
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
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "File " + filePath + " could not be opened", __FUNCTION__);
      return nullptr;
    }

    // get size of file
    ifs.seekg(0, ifs.end);
    fileSize = ifs.tellg();
    ifs.seekg(0, ifs.beg);

    if (fileSize == 0) {
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "File being read is empty!", __FUNCTION__);
      return nullptr;
    }

    char* fileBuffer = new char[fileSize];
    ifs.read(fileBuffer, fileSize);
    if (!ifs) {
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

//#ifdef _DEBUG
    PrintMonoAssemblyTypes(monoAssembly);
//#endif

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

      std::string nsName{ mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]) };
      std::string className{ mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]) };

      //std::cout << "Namespace: " << nsName << "\n";
      //std::cout << "Class Name: " << className << "\n";
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Namespace: " + nsName, __FUNCTION__);
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Class Name: " + className, __FUNCTION__);

      if (nsName == "Object") {
        std::string combinedName{ nsName + className };
        sAssignableScriptNames.push_back(_strdup(combinedName.c_str()));
      }
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
      //for (auto const& pair : sEntityClasses) {
      //    ScriptClass const& value{ pair.second };
      //  //std::cout << "Key: " << pair.first << ", Namespace: " << value.GetNamespace() 
      //  //  << ", Class: " << value.GetClassName() << "\n";
      //}
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

  Function is deprecated for the OnCreateEntityEvent function which uses
  subscriber callback instead.
  */
  void ScriptManager::OnCreateEntity(Entity const& entity) {
    ::gCoordinator = Coordinator::GetInstance();
    auto const& scriptComp{ gCoordinator->GetComponent<Script>(entity) };
    if (EntityClassExists(scriptComp.name)) {
      ScriptInstance si{ sEntityClasses[scriptComp.name], entity };
      sEntityInstances[entity] = si;
      si.CallOnCreate();

      //std::cout << "Entity w script component named " << scriptComp.name << " created!" << "\n";
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Entity w script component named " + 
        scriptComp.name + " created!", __FUNCTION__);
    }
    else {
      //std::cout << "Entity Script does not exist!" << "\n";
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Entity Script does not exist!"
        , __FUNCTION__);
    }
  }


  namespace Hack {
    std::map<Entity, std::string> entitiesScripted;
  }
  /*  _________________________________________________________________________ */
  /*! OnCreateEntityEvent

  @param event
  The event that contains the entity handle for the created entity with a 
  script component.

  @return none.

  Function to be called after creating an entity with script component.
  Creates a script instance and maps it to the entity ID, then calls its
  own create function.
  */
  void ScriptManager::OnCreateEntityEvent(Event& event) {
      ::gCoordinator = Coordinator::GetInstance();
      Entity entity{ event.GetParam<Entity>(Events::System::Entity::COMPONENT_ADD) };
      if (event.GetFail()) return;
      if (!gCoordinator->HasComponent<Script>(entity)) return;

      if (Hack::entitiesScripted.find(entity) != Hack::entitiesScripted.end()) return;
      Hack::entitiesScripted[entity] = gCoordinator->GetComponent<Script>(entity).name;

      auto const& scriptComp{ gCoordinator->GetComponent<Script>(entity) };
      if (EntityClassExists(scriptComp.name)) {
        ScriptInstance si{ sEntityClasses[scriptComp.name], entity };
        sEntityInstances[entity] = si;
        si.CallOnCreate();

        //std::cout << "Entity w script component named " << scriptComp.name << " created!" << "\n";
        LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Entity w script component named " +
          scriptComp.name + " created!", __FUNCTION__);
      }
      else {
        //std::cout << "Entity Script does not exist!" << "\n";
        LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Entity Script does not exist!"
          , __FUNCTION__);
      }

      //every item added to the ecs will be marked as an entity to serialize
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
  /*! RemoveEntity

  @param entity
  The entity handle for the created entity with a script component.

  @return none.

  Removes the entity from the map.
  */
  void ScriptManager::RemoveEntity(Entity const& entity) {
    sEntityInstances.erase(entity);
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

  /*  _________________________________________________________________________ */
  /*! FillAssignableScriptNames

  @return none.

  Fills the vector of assignable script names.
  */
  /*
  void ScriptManager::FillAssignableScriptNames() {
    
  }
  */
}