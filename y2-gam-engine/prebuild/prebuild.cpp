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
    std::ofstream ofs{"include/Core/Serialization/Serializer.hpp"};
    std::ofstream ofs1{"include/Components/Component.hpp"};
    writeline(ofs, "#pragma once");
    writeline(ofs, "#include <Core/Types.hpp>");
    writeline(ofs, "#include <functional>");
    writeline(ofs, "#include <Core/Coordinator.hpp>");
    writeline(ofs, "#include <rapidjson/document.h>");
    writeline(ofs, "#include <string>");
    writeline(ofs, "#include <map>");

    //start namespace

    std::vector<std::string> componentNames{};
    //create the names + includes
    #define writehpp(file) writeline(ofs, "#include <Components/" + file + ">")
    #define writehpp1(file) writeline(ofs1, "#include <Components/" + file + ">")
    #define closebrace writeline(ofs, "}");
    for (auto const& dirEntry : std::filesystem::directory_iterator{"include/Components"}){
        writehpp(dirEntry.path().filename().string());
        if (dirEntry.path().filename().string() != "Component.hpp") writehpp1(dirEntry.path().filename().string());
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
        writeline(ofs, "if constexpr (std::is_same_v<T, "+component+">) return \""+component+"\";");
    }
    writeline(ofs, "return \"NULL\";");
    closebrace;

    writeline(ofs, "static void SerializeEntity(Entity const& entity, rapidjson::Value& obj) {");
    for (std::string const& component: componentNames){
        writeline(ofs, "if (Coordinator::GetInstance()->HasComponent<"+component+">(entity))");
        writeline(ofs, "Coordinator::GetInstance()->GetComponent<"+component+">(entity).Serialize(obj);");
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
    
}