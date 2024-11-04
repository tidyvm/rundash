#include "conf.h"

#include <fstream>

template <>
int StringMap::convert(std::string e) { return atoi(e.c_str()); }

StringMap parseConfFile(const char* path) {
    StringMap result;
    std::ifstream f(path);
    std::string line;
    while(std::getline(f, line)) {
        if(line[0] == '#')
            continue;
        size_t p = line.find('=');
        if(p != std::string::npos) {
            result.emplace(line.substr(0, p), line.substr(p+1));
        }
    }
    return result;
}
