/******************************************************************************/
/*!
\par        Image Engine
\file       ScriptManager.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Dec 24, 2023

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
#include "Engine/StateManager.hpp"

namespace {
  std::shared_ptr<Coordinator> gCoordinator;
  static int num = 0;
}

namespace Image {

  MonoDomain* ScriptManager::sRootDomain{ nullptr };
  MonoDomain* ScriptManager::sAppDomain{ nullptr };
  std::string ScriptManager::sMainAssemblyFilePath{};
#ifndef _INSTALLER
  std::unique_ptr<filewatch::FileWatch<std::string>> ScriptManager::mAppAssemblyFileWatcher;
#endif
  std::unordered_map<std::string, ScriptClass> ScriptManager::sEntityClasses{};
  std::map<Entity, ScriptInstance> ScriptManager::sEntityInstances{};
  std::map <std::string, ScriptInstance> ScriptManager::sTagToRawInstances{};
  std::vector<const char*> ScriptManager::sAssignableScriptNames{};

  std::map<std::string, Field> ScriptManager::sScriptFieldTypes {
    { "System.Single", {FieldType::Float, nullptr} },
    { "System.Double", {FieldType::Double, nullptr}  },
    { "System.Boolean", {FieldType::Bool, nullptr}  },
    { "System.Int16", {FieldType::Short, nullptr}  },
    { "System.Int32", {FieldType::Int, nullptr}  },
    { "System.Int64", {FieldType::Long, nullptr}  },
    { "System.UInt16", {FieldType::UShort, nullptr}  },
    { "System.UInt32", {FieldType::UInt, nullptr}  },
    { "System.UInt64", {FieldType::ULong, nullptr}  },
    { "Image.Vector2", {FieldType::Vector2, nullptr}  },
    { "Image.Vector3", {FieldType::Vector3, nullptr}  },
    { "Image.Vector4", {FieldType::Vector4, nullptr}  },
    { "Image.Entity", {FieldType::Entity, nullptr}  },
    { "System.String", {FieldType::String, nullptr} }
  };
  bool ScriptManager::AssemblyReloadPending{ false };

#ifndef _INSTALLER
  void ScriptManager::AssemblyFileSystemEvent(std::string const& filePath, filewatch::Event const change_type) {
    if (!ScriptManager::AssemblyReloadPending && change_type == filewatch::Event::modified) {
      ScriptManager::AssemblyReloadPending = true;

      std::shared_ptr<StateManager> gStateManager = StateManager::GetInstance();
      gStateManager->SubmitToMainThread([]()
      {
        ScriptManager::mAppAssemblyFileWatcher.reset();
        ScriptManager::ReloadAssembly();
      });
    }
  }
#endif

  /*  _________________________________________________________________________ */
  /*! Init

  @return none.

  The main init. Calls the respective script related functions.
  */
  void ScriptManager::Init() {
    sMainAssemblyFilePath = "../assets/scripts/y2-gam-script.dll";

#ifndef _INSTALLER
    mAppAssemblyFileWatcher = std::make_unique<filewatch::FileWatch<std::string>>(
      "../assets/scripts", AssemblyFileSystemEvent);
#endif
    AssemblyReloadPending = false;
    InitMono();
    ScriptCoordinator::RegisterFunctions();
    MonoAssembly* ma{ Image::ScriptManager::LoadCSharpAssembly(sMainAssemblyFilePath) };
    Image::ScriptManager::PopulateEntityClassesFromAssembly(ma);

#ifndef _INSTALLER
    /*
    mAppAssemblyFileWatcher = std::make_unique<filewatch::FileWatch<std::string>>(
      "../assets/scripts",
      [](const std::string& path, const filewatch::Event change_type) {
      std::cout << "File: " << path << " Event: ";
      switch (change_type)
      {
      case filewatch::Event::added:
        std::cout << "The file was added to the directory." << '\n';
        break;
      case filewatch::Event::removed:
        std::cout << "The file was removed from the directory." << '\n';
        break;
      case filewatch::Event::modified:
        std::cout << "The file was modified. This can be a change in the time stamp or attributes." << '\n';
        break;
      case filewatch::Event::renamed_old:
        std::cout << "The file was renamed and this is the old name." << '\n';
        break;
      }
      }
    );
    */
#endif
  }

  /*  _________________________________________________________________________ */
  /*! Exit

  @return none.

  The main exit.
  */
  void ScriptManager::Exit() {
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
    mono_set_assemblies_path("lib/mono");

    // Mono auto detects the runtime version based on first assembly loaded
    MonoDomain* rootDomain{ mono_jit_init("ScriptRuntime") };

    if (rootDomain == nullptr) {
#ifndef _INSTALLER
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "Root Domain Initialization Failed!", __FUNCTION__);
#endif
      return;
    }
    sRootDomain = rootDomain;

    char appDomainName[] = "MyAppDomain";
    // An app domain is a C# language feature
    MonoDomain* appDomain = mono_domain_create_appdomain(appDomainName, nullptr);
    if (appDomain == nullptr) {
#ifndef _INSTALLER
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "App Domain Initialization Failed!", __FUNCTION__);
#endif
      return;
    }
    sAppDomain = appDomain;

    mono_domain_set(sAppDomain, true);
    Coordinator::GetInstance()->AddEventListener(FUNCTION_LISTENER(Events::System::ENTITY, ScriptManager::OnCreateEntityEvent));
  }

  /*  _________________________________________________________________________ */
  /*! ExitMono

  @return none.

  The clean up function for exiting script related components.
  */
  void ScriptManager::ExitMono() {
    for (auto const& e : GetEntityInstances()) {
      Image::ScriptManager::OnExitEntity(e.first);
    }

    for (const char* str : sAssignableScriptNames) {
      free((void*)str); // Free the duplicated strings
    }
    sAssignableScriptNames.clear(); // Clear the vector

    Exit();
  }

  /*  _________________________________________________________________________ */
  /*! GetEntityScriptInstance
  
  @param entity
  The entity with a script component.

  @return ScriptInstance&
  Reference to the script instance.

  Gets the script instance of a particular entity (that has the script component).
  */
  ScriptInstance& ScriptManager::GetEntityScriptInstance(Entity const& entity) {
    return sEntityInstances[entity];
  }

  ScriptInstance& ScriptManager::GetTagToRawScriptInstance(std::string const& tag) {
    return sTagToRawInstances[tag];
  }

  ScriptInstance& ScriptManager::CreateScriptInstanceWithTag(std::string const& scriptName, std::string const& tag) {
    if (EntityClassExists(scriptName)) {
      sTagToRawInstances[tag] = { ScriptInstance(sEntityClasses[scriptName]) };
      std::cout << "Raw Instance for: " << scriptName << " created!\n";
      return sTagToRawInstances[tag];
    }
  }

  /*  _________________________________________________________________________ */
  /*! GetEntityMonoInstanceObject

  @param entity
  The entity with a script component.

  @return MonoObject*

  Gets the mono instance object.
  */
  MonoObject* ScriptManager::GetEntityMonoInstanceObject(Entity const& entity) {
    return GetEntityScriptInstance(entity).GetMonoInstanceObject();
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
#ifndef _INSTALLER
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "File " + filePath + " could not be opened", __FUNCTION__);
#endif
      return nullptr;
    }

    // get size of file
    ifs.seekg(0, ifs.end);
    fileSize = ifs.tellg();
    ifs.seekg(0, ifs.beg);

    if (fileSize == 0) {
#ifndef _INSTALLER
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "File being read is empty!", __FUNCTION__);
#endif
      return nullptr;
    }

    char* fileBuffer = new char[fileSize];
    ifs.read(fileBuffer, fileSize);
    if (!ifs) {
#ifndef _INSTALLER
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "File reading error!", __FUNCTION__);
#endif
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

    PrintMonoAssemblyTypes(monoAssembly);

    return monoAssembly;
  }

  void ScriptManager::ReloadAssembly() {
    std::cout << "Assembly Reloading\n";
    mono_domain_set(mono_get_root_domain(), false);
    mono_domain_unload(sAppDomain);

    char appDomainName[] = "MyAppDomain";
    // An app domain is a C# language feature
    MonoDomain* appDomain = mono_domain_create_appdomain(appDomainName, nullptr);
    if (appDomain == nullptr) {
#ifndef _INSTALLER
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "App Domain Initialization Failed!", __FUNCTION__);
#endif
      return;
    }
    sAppDomain = appDomain;

    mono_domain_set(sAppDomain, true);
    Coordinator::GetInstance()->AddEventListener(FUNCTION_LISTENER(Events::System::ENTITY, ScriptManager::OnCreateEntityEvent));

#ifndef _INSTALLER
    mAppAssemblyFileWatcher = std::make_unique<filewatch::FileWatch<std::string>>(
      "../assets/scripts", AssemblyFileSystemEvent);
#endif
    AssemblyReloadPending = false;
    ScriptCoordinator::RegisterFunctions();
    MonoAssembly* ma{ Image::ScriptManager::LoadCSharpAssembly(sMainAssemblyFilePath) };
    Image::ScriptManager::PopulateEntityClassesFromAssembly(ma);
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

#ifndef _INSTALLER
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Namespace: " + nsName, __FUNCTION__);
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Class Name: " + className, __FUNCTION__);
#endif

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
        void* iter = nullptr;
        while (MonoClassField* field = mono_class_get_fields(monoClass, &iter)) {
          const char* fieldName = mono_field_get_name(field);
          uint32_t flags = mono_field_get_flags(field);
          if (flags & FIELD_ATTRIBUTE_PUBLIC) {
            MonoType* type = mono_field_get_type(field);
            std::cout << "Name: " << fieldName << ", Type: " << FieldTypeToString(MonoToScriptType(type)) << "\n";

            sc.mFieldNameToTypeMap[fieldName] = { MonoToScriptType(type), field };
          }
        }

        sEntityClasses[finalName] = sc;
      }
    }
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
      if (!::gCoordinator->HasComponent<Script>(entity)) return;

      if (Hack::entitiesScripted.find(entity) != Hack::entitiesScripted.end()) return;
      Hack::entitiesScripted[entity] = ::gCoordinator->GetComponent<Script>(entity).name;
  }

  void ScriptManager::LoadEntityLinkage(Entity entity, std::string tag) {
    auto const& scriptComp{ ::gCoordinator->GetComponent<Script>(entity) };
    auto it = sTagToRawInstances.find(tag);

    if (it != sTagToRawInstances.end()) {
      sEntityInstances[entity] = sTagToRawInstances[tag];
      sEntityInstances[entity].CallOnCreate(entity);

#ifndef _INSTALLER
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Entity w script component named " +
        scriptComp.name + " created!", __FUNCTION__);
#endif
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
  /*! OnExitEntity

  @param entity
  The entity handle for the created entity with a script component.

  @return none.

  This function is called on exit for the entity.
  */
  void ScriptManager::OnExitEntity(Entity const& entity) {
    sEntityInstances[entity].CallOnExit();
  }

  /*  _________________________________________________________________________ */
  /*! RemoveEntity

  @param entity
  The entity handle for the created entity with a script component.

  @return none.

  Removes the entity from the map.
  */
  void ScriptManager::RemoveEntity(Entity const& entity) {
    auto it = sEntityInstances.find(entity);
    if (it != sEntityInstances.end()) {
      sEntityInstances.erase(it);
      if (::gCoordinator->HasComponent<Tag>(entity)) {
        sTagToRawInstances.erase(::gCoordinator->GetComponent<Tag>(entity).tag);
      }
    }
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
  /*! PrintEntityInstances

  @return none.

  Prints all the entity script pairs in the script instance map.
  */
  void ScriptManager::PrintEntityInstances() {
    std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
    std::cout << "Entity-Script Pairs in the Script Instance Map:\n";
    for (const auto& pair : sEntityInstances) {
      std::cout << pair.first << ": " << ::gCoordinator->GetComponent<Script>(pair.first).name << "\n";
    }
    std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
  }

  /*  _________________________________________________________________________ */
  /*! MonoToScriptType

  @param monoType
  The mono type.

  @return FieldType
  The field type.

  Converts a monoType to a FieldType.
  */
  FieldType ScriptManager::MonoToScriptType(MonoType* monoType) {
    const char* typeName = mono_type_get_name(monoType);
    return sScriptFieldTypes[typeName].fieldType;
  }

  /*  _________________________________________________________________________ */
  /*! FieldTypeToString

  @param fieldType
  The field type.

  @return std::string

  Converts a FieldType to a std::string.
  */
  std::string ScriptManager::FieldTypeToString(FieldType fieldType) {
    switch (fieldType) {
    case FieldType::None:    return "None";
    case FieldType::Float:   return "Float";
    case FieldType::Double:  return "Double";
    case FieldType::Bool:    return "Bool";
    case FieldType::Short:   return "Short";
    case FieldType::Int:     return "Int";
    case FieldType::Long:    return "Long";
    case FieldType::UShort:  return "UShort";
    case FieldType::UInt:    return "UInt";
    case FieldType::ULong:   return "ULong";
    case FieldType::Vector2: return "Vector2";
    case FieldType::Vector3: return "Vector3";
    case FieldType::Vector4: return "Vector4";
    case FieldType::Entity:  return "Entity";
    case FieldType::String:  return "String";
    }
    return "None";
  }
}