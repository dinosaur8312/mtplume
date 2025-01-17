
#include "JsonInputHandler.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

// Initialize static members
std::string JsonInputHandler::basePath = "../tests/unit_tests/";
std::string JsonInputHandler::currentPath = "./";

JsonInputHandler::JsonInputHandler(const std::string& inputFileName) {
    // Constructor logic here (if needed)
}

SimConfig JsonInputHandler::processJsonFile(const std::string& filePath) {
    std::string resolvedPath = findJsonFile(filePath);
    //print the path
    std::cout << "Input Json Config File: " << resolvedPath << std::endl;
    std::ifstream file(resolvedPath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open JSON input file: " + resolvedPath);
    }

    nlohmann::json j;
    file >> j;
    file.close();

    SimConfig config;

    config.coefCSVPath = j.value("coefs_CSV", "");
    config.refCSVPath = j.value("reference_CSV", "");
    config.outputCSVPath = j.value("outputCSV", "");
    // Set computeMode based on filename
    if (config.refCSVPath.find("_02") != std::string::npos) 
    {
        config.computeMode = 1;
    } 
    else if (config.refCSVPath.find("zFunc") != std::string::npos) 
    {
        config.computeMode = 2;
    }
    else if (config.refCSVPath.find("sample") != std::string::npos)
    {
        config.computeMode = 3;
    }
    else if (config.refCSVPath.find("_01") != std::string::npos) 
    {
        config.computeMode = 0;
    }
    else if (config.refCSVPath.find("source_sigma") != std::string::npos) 
    {
        config.computeMode = 4;
    }
    else
    {
        config.computeMode = 3;
    }
    //print the path
    std::cout << "input coef CSV: " << config.coefCSVPath << std::endl;
    std::cout << "input ref CSV: " << config.refCSVPath << std::endl;
    std::cout << "outputCSV: " << config.outputCSVPath << std::endl;
    // Populate other fields as necessary

    return config;
}

SimConfig JsonInputHandler::processJsonFile(const std::string& filePath) {
    // Existing file resolution and JSON parsing code remains unchanged

    SimConfig config;
    // Existing path and config setup code remains unchanged

    // Process parameters within reference_CSV
    auto& params = j["reference_CSV"]["parameters"];
    for (auto& [key, value] : params.items()) {
        if (value.value("required", false)) {
            // Assume getParamKey is a function that converts a string key to ParamKey
            ParamKey paramKey = getParamKey(key); 
            config.parameters[paramKey] = {value.value("columnName", "")};
        }
    }

    // ComputeMode logic and printing paths remain unchanged

    return config;
}

std::string JsonInputHandler::findJsonFile(const std::string& fileName) {
    std::string defaultFileName = "input_default.json";
    if (!fileName.empty()) {
        if (fs::exists(currentPath + fileName)) {
            return currentPath + fileName;
        } else if (fs::exists(basePath + fileName)) {
            return basePath + fileName;
        }
    }
    // Fallback to default file if fileName is empty or not found
    return basePath + defaultFileName;
}
