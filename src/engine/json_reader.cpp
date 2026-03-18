#include "engine/json_reader.h"
using json = nlohmann::json;

JsonFile::JsonFile(const std::string& path): 
    dataFile([&]() {
        std::ifstream file(path);

        if (!file.is_open()) {
            throw std::runtime_error("Could not open JSON file: " + path);
        }

        return json::parse(file);
    }())
{}