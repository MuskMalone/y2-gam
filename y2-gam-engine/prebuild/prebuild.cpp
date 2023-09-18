#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
void writeline(std::ofstream &ofs, std::string line){
    ofs << line << std::endl;
}
int main(){
    std::ofstream ofs{"include/Core/Serialization/Test.hpp"};
    writeline(ofs, "#pragma once");
    writeline(ofs, "#include <Core/Types.hpp>");
}