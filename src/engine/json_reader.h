#pragma once
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
using json = nlohmann::json;

class jsonFile {
public:
    jsonFile(const std::string& path);
    const json dataFile;

    template <typename T>
    T get(const std::string& key) const {
        return dataFile.at(key).get<T>();
    }
};