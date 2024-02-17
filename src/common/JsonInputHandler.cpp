#include "JsonInputHandler.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

JsonInputHandler::JsonInputHandler(const std::string& inputFileName) {
    inputJsonPath = findJsonFile(inputFileName);
    if (inputJsonPath.empty()) {
        throw std::runtime_error("JSON input file not found.");
    }
    processJsonFile(inputJsonPath);
}

void JsonInputHandler::readInput() {
    // Here you can implement logic that uses inputCSVPath and outputCSVPath
    // For example, reading the CSV file, processing it, and writing to the output CSV file.
}

std::string JsonInputHandler::findJsonFile(const std::string& fileName) {
    if (fs::exists(currentPath + fileName)) {
        return currentPath + fileName;
    } else if (fs::exists(basePath + fileName)) {
        return basePath + fileName;
    } else if (fileName.empty()) { // Use default if no argument provided
        return basePath + "input_default.json";
    }
    return ""; // Return empty string if file not found
}

std::string JsonInputHandler::resolveFilePath(const std::string& fileName, bool searchInTests) {
    if (fs::path(fileName).is_absolute()) {
        return fileName; // Use absolute path directly
    }

    auto currentDirPath = currentPath + fileName;
    auto testDirPath = basePath + fileName;
    if (fs::exists(currentDirPath)) {
        return currentDirPath;
    } else if (searchInTests && fs::exists(testDirPath)) {
        return testDirPath;
    }
    throw std::runtime_error("CSV file not found: " + fileName);
}

void JsonInputHandler::processJsonFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open JSON input file: " + filePath);
    }

    nlohmann::json j;
    file >> j;

    // Resolve paths for inputCSV and outputCSV
    if (!j.contains("inputCSV") || j["inputCSV"].get<std::string>().empty()) {
        throw std::runtime_error("inputCSV key is missing or blank.");
    }
    inputCSVPath = resolveFilePath(j["inputCSV"].get<std::string>());

    if (!j.contains("outputCSV") || j["outputCSV"].get<std::string>().empty()) {
        throw std::runtime_error("outputCSV key is missing or blank.");
    }
    outputCSVPath = resolveFilePath(j["outputCSV"].get<std::string>(), false);
}
