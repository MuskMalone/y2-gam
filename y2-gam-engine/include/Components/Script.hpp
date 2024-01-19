/******************************************************************************/
/*!
\par        Image Engine
\file       Script.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Dec 26, 2023

\brief      The main script component. Also handles the serialization of
						public script variables from the respective scripts.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <string>
#include <Core/Serialization/SerializationManager.hpp>
#include "Systems/EntitySerializationSystem.hpp"
#include "Scripting/ScriptManager.hpp"
#include "Scripting/ScriptInstance.hpp"
#include "Components/Tag.hpp"

struct Script {
  std::string name;
	std::string scriptTagged;

	Script() = default;

	Script(std::string scriptName, std::string tag) : name{ scriptName }, scriptTagged{ tag } {}

	Script(rapidjson::Value const& obj) {
    std::cout << "This got called\n";
		name = obj["scriptName"].GetString();
		scriptTagged = obj["scriptTag"].GetString();

    if (name != "No Script Assigned") {
      ScriptInstance scriptInstance{ ScriptManager::CreateScriptInstanceWithTag(name, scriptTagged) };
      std::map<std::string, Image::Field> const& fields{ scriptInstance.GetScriptClass().GetFieldNameToTypeMap() };

      // Retrieve Values
      for (std::pair<std::string, Image::Field> const& val : fields) {
        
        if (!obj.HasMember(val.first.c_str())) {
          //std::cout << val.first.c_str() << "\n";
          continue;
        }
        
        switch (val.second.fieldType) {

        case Image::FieldType::None: {
          break;
        }

        case Image::FieldType::Float: {
          float dataFloat{ obj[val.first.c_str()].GetFloat() };
          scriptInstance.SetFieldValueWithName(val.first, dataFloat);
          break;
        }

        case Image::FieldType::Double: {
          double dataDouble{ obj[val.first.c_str()].GetDouble() };
          scriptInstance.SetFieldValueWithName(val.first, dataDouble);
          break;
        }

        case Image::FieldType::Bool: {
          bool dataBool{ obj[val.first.c_str()].GetBool() };
          scriptInstance.SetFieldValueWithName(val.first, dataBool);
          break;
        }

        case Image::FieldType::Short: {
          short dataShort{ static_cast<short>(obj[val.first.c_str()].GetInt()) };
          scriptInstance.SetFieldValueWithName(val.first, dataShort);
          break;
        }

        case Image::FieldType::Int: {
          int dataInt{ obj[val.first.c_str()].GetInt() };
          scriptInstance.SetFieldValueWithName(val.first, dataInt);
          break;
        }

        case Image::FieldType::Long: {
          long dataLong{ static_cast<long>(obj[val.first.c_str()].GetInt64()) };
          scriptInstance.SetFieldValueWithName(val.first, dataLong);
          break;
        }

        case Image::FieldType::UShort: {
          unsigned short dataUShort{ static_cast<unsigned short>(obj[val.first.c_str()].GetUint()) };
          scriptInstance.SetFieldValueWithName(val.first, dataUShort);
          break;
        }

        case Image::FieldType::UInt: {
          unsigned int dataUInt{ obj[val.first.c_str()].GetUint() };
          scriptInstance.SetFieldValueWithName(val.first, dataUInt);
          break;
        }

        case Image::FieldType::ULong: {
          unsigned long dataULong{ static_cast<unsigned long>(obj[val.first.c_str()].GetUint64()) };
          scriptInstance.SetFieldValueWithName(val.first, dataULong);
          break;
        }

        case Image::FieldType::Vector2: {
          Vec2 dataVec2{ Vec2(obj[(val.first + "X").c_str()].GetFloat(),
            obj[(val.first + "Y").c_str()].GetFloat()) };
          scriptInstance.SetFieldValueWithName(val.first, dataVec2);
          break;
        }

        case Image::FieldType::Vector3: {
          Vec3 dataVec3{ Vec3(obj[(val.first + "X").c_str()].GetFloat(),
            obj[(val.first + "Y").c_str()].GetFloat(),
            obj[(val.first + "Z").c_str()].GetFloat()) };
          scriptInstance.SetFieldValueWithName(val.first, dataVec3);
          break;
        }

        case Image::FieldType::Vector4: {
          Vec4 dataVec4{ Vec4(obj[(val.first + "X").c_str()].GetFloat(),
            obj[(val.first + "Y").c_str()].GetFloat(),
            obj[(val.first + "Z").c_str()].GetFloat(),
            obj[(val.first + "W").c_str()].GetFloat()) };
          scriptInstance.SetFieldValueWithName(val.first, dataVec4);
          break;
        }

        case Image::FieldType::Entity: {
          Entity dataEntity{ static_cast<Entity>(obj[val.first.c_str()].GetInt()) };
          scriptInstance.SetFieldValueWithName(val.first, dataEntity);
          break;
        }
        }
      }
    }
	}

	bool Serialize(rapidjson::Value& obj) {
		std::shared_ptr<Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };
		sm->InsertValue(obj, "scriptName", name);
		sm->InsertValue(obj, "scriptTag", scriptTagged);

    if (name != "No Script Assigned") {
      ScriptInstance& scriptInstance{ ScriptManager::GetTagToRawScriptInstance(scriptTagged) };
      std::map<std::string, Image::Field> const& fields{ scriptInstance.GetScriptClass().GetFieldNameToTypeMap() };

      // Insert the values
      for (std::pair<std::string, Image::Field> const& val : fields) {
        switch (val.second.fieldType) {

        case Image::FieldType::None: {
          break;
        }

        case Image::FieldType::Float: {
          float dataFloat{ scriptInstance.GetFieldValueFromName<float>(val.first) };
          sm->InsertValue(obj, val.first, dataFloat);
          break;
        }

        case Image::FieldType::Double: {
          double dataDouble{ scriptInstance.GetFieldValueFromName<double>(val.first) };
          sm->InsertValue(obj, val.first, dataDouble);
          break;
        }

        case Image::FieldType::Bool: {
          bool dataBool{ scriptInstance.GetFieldValueFromName<bool>(val.first) };
          sm->InsertValue(obj, val.first, dataBool);
          break;
        }

        case Image::FieldType::Short: {
          short dataShort{ scriptInstance.GetFieldValueFromName<short>(val.first) };
          sm->InsertValue(obj, val.first, dataShort);
          break;
        }

        case Image::FieldType::Int: {
          int dataInt{ scriptInstance.GetFieldValueFromName<int>(val.first) };
          sm->InsertValue(obj, val.first, dataInt);
          break;
        }

        case Image::FieldType::Long: {
          INT64 dataLong{ scriptInstance.GetFieldValueFromName<long>(val.first) };
          sm->InsertValue(obj, val.first, dataLong);
          break;
        }

        case Image::FieldType::UShort: {
          unsigned short dataUShort{ scriptInstance.GetFieldValueFromName<unsigned short>(val.first) };
          sm->InsertValue(obj, val.first, dataUShort);
          break;
        }

        case Image::FieldType::UInt: {
          unsigned int dataUInt{ scriptInstance.GetFieldValueFromName<unsigned int>(val.first) };
          sm->InsertValue(obj, val.first, dataUInt);
          break;
        }

        case Image::FieldType::ULong: {
          UINT64 dataULong{ scriptInstance.GetFieldValueFromName<unsigned long>(val.first) };
          sm->InsertValue(obj, val.first, dataULong);
          break;
        }

        case Image::FieldType::Vector2: {
          Vec2 dataVec2{ scriptInstance.GetFieldValueFromName<Vec2>(val.first) };
          sm->InsertValue(obj, val.first + "X", dataVec2.x);
          sm->InsertValue(obj, val.first + "Y", dataVec2.y);
          break;
        }

        case Image::FieldType::Vector3: {
          Vec3 dataVec3{ scriptInstance.GetFieldValueFromName<Vec3>(val.first) };
          sm->InsertValue(obj, val.first + "X", dataVec3.x);
          sm->InsertValue(obj, val.first + "Y", dataVec3.y);
          sm->InsertValue(obj, val.first + "Z", dataVec3.z);
          break;
        }

        case Image::FieldType::Vector4: {
          Vec4 dataVec4{ scriptInstance.GetFieldValueFromName<Vec4>(val.first) };
          sm->InsertValue(obj, val.first + "X", dataVec4.x);
          sm->InsertValue(obj, val.first + "Y", dataVec4.y);
          sm->InsertValue(obj, val.first + "Z", dataVec4.z);
          sm->InsertValue(obj, val.first + "W", dataVec4.w);
          break;
        }

        case Image::FieldType::Entity: {
          Entity dataEntity{ scriptInstance.GetFieldValueFromName<Entity>(val.first) };
          sm->InsertValue(obj, val.first, dataEntity);
          break;
        }
        }
      }
    }

		return true;
	}
};