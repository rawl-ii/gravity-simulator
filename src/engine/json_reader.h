#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class JsonFile {
public:
    JsonFile(const std::string& path);
    const json dataFile;

    template <typename T>
    T get(const std::string& key) const {
        return dataFile.at(key).get<T>();
    }
};