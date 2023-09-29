/******************************************************************************/
/*!
\par        Prebuild
\file       prebuild.cpp

\author     Ng Wen Wen (wenwen.ng@digipen.edu)
\date       Sep 6, 2023

\brief      This file's code automates the management of JSON serialization of 
            entities with components

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
void writeline(std::ofstream &ofs, std::string line){
    ofs << line << std::endl;
}

#define getfilename(filename) filename.substr(0, filename.find_first_of('.'))
int main(){
    namespace fs = std::filesystem;

    try {
        for (const auto &entry : fs::directory_iterator(fs::current_path())) {
            std::cout << entry.path() << '\n';
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << e.what() << '\n';
    }    
    std::ofstream ofs{"include/Core/Serialization/Serializer.hpp"};
    writeline(ofs, "#pragma once");
    writeline(ofs, "#include <Core/Types.hpp>");
    writeline(ofs, "#include <functional>");
    writeline(ofs, "#include <Core/Coordinator.hpp>");
    writeline(ofs, "#include <rapidjson/document.h>");
    writeline(ofs, "#include <string>");
    writeline(ofs, "#include <map>");
    writeline(ofs, "#include <Core/Serialization/SerializationManager.hpp>");

    //start namespace

    std::vector<std::string> componentNames{};
    //create the names + includes
    #define writehpp(file) writeline(ofs, "#include <Components/" + file + ">")
    #define writehpp1(file) writeline(ofs1, "#include <Components/" + file + ">")
    #define closebrace writeline(ofs, "}");
    for (auto const& dirEntry : std::filesystem::directory_iterator{"include/Components"}){
        writehpp(dirEntry.path().filename().string());
                componentNames.push_back(getfilename(dirEntry.path().filename().string()));

    }
    writeline(ofs, "namespace Serializer{");
    //create the functions to add component
    for (std::string const& component : componentNames){
        writeline(ofs, "static void EntityAdd"+component+"(Entity const& entity, rapidjson::Value const& obj) {");
        writeline(ofs, "Coordinator::GetInstance()->AddComponent(entity, "+component+"{ obj });");
        closebrace;
    }
    
    //write type to string
    writeline(ofs, "template <typename _type> std::string TypeToString() {");
    for (std::string const& component : componentNames){
        writeline(ofs, "if constexpr (std::is_same_v<_type, "+component+">) return \""+component+"\";");
    }
    writeline(ofs, "return \"NULL\";");
    closebrace;

    writeline(ofs, "static void SerializeEntity(Entity const& entity, JSONObj& ent) {");
    for (std::string const& component: componentNames){
        writeline(ofs, "if (Coordinator::GetInstance()->HasComponent<"+component+">(entity)){");
        writeline(ofs, "JSONObj obj{ JSON_OBJ_TYPE };");
        writeline(ofs, "obj.SetObject();");

        writeline(ofs, "bool res = Coordinator::GetInstance()->GetComponent<"+component+">(entity).Serialize(obj);");
        writeline(ofs, "if (res) { SerializationManager::GetInstance()->InsertValue(ent, TypeToString<"+component+">(), obj); }");
        writeline(ofs, "else { obj.SetNull(); }");
        closebrace;
    }
    closebrace;

    //declare map
    writeline(ofs, "static const std::map<std::string, std::function<void(Entity const&, rapidjson::Value const&)>> gComponentSerializer{");
    size_t i{1};
    for (std::string const& component : componentNames){
        writeline(ofs, "{\""+component+"\", EntityAdd"+component+"}" + ((i == componentNames.size()) ? "" : ","));
        i++;
    }
    writeline(ofs, "};");

    //end namespace
    closebrace;

    std::ofstream ofs1{"include/Core/Component.hpp"};

    #define writehpp1(file) writeline(ofs1, "#include <Components/" + file + ">")
    for (auto const& dirEntry : std::filesystem::directory_iterator{"include/Components"}){
        if (dirEntry.path().filename().string() != "Component.hpp") writehpp1(dirEntry.path().filename().string());
    }


    
}
