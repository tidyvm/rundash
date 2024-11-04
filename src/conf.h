#ifndef LAMINAR_CONF_H_
#define LAMINAR_CONF_H_

#include <string>
#include <unordered_map>

class StringMap : public std::unordered_map<std::string, std::string> {
public:
    template<typename T>
    T get(std::string key, T fallback = T()) {
        auto it = find(key);
        return it != end() ? convert<T>(it->second) : fallback;
    }
private:
    template<typename T>
    T convert(std::string e) { return e; }
};
template <>
int StringMap::convert(std::string e);

// Reads a file by line into a list of key/value pairs
// separated by the first '=' character. Discards lines
// beginning with '#'
StringMap parseConfFile(const char* path);


#endif // LAMINAR_CONF_H_
