#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

class JsonInputHandler {
public:
    explicit JsonInputHandler(const std::string& inputFileName);
    void readInput();

private:
    std::string inputJsonPath;
    std::string inputCSVPath;
    std::string outputCSVPath;
    std::string basePath = "./tests/unit_tests/";
    std::string currentPath = "./";

    std::string findJsonFile(const std::string& fileName);
    std::string resolveFilePath(const std::string& fileName, bool searchInTests = true);
    void processJsonFile(const std::string& filePath);
};
