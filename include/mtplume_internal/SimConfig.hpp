#pragma once

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp> // Include this for JSON processing

// Define an enum class for parameter keys
enum class ParamKey {
    Stability,
    WindSpeed,
    Hml,
    Q,
    SrcX,
    SrcY,
    SrcZ,
    RecX,
    RecY,
    RecZ,
    SigX,
    SigY,
    SigZ
    // Add other parameters as needed
};

// Simplify ParameterConfig to only store column names
struct ParameterConfig {
    std::string columnName;
};

class SimConfig {
public:
    std::string coefPath;
    std::string inputPath;
    std::string outputPath;
    std::unordered_map<ParamKey, std::string> parameters; // Store just column names
    int computeMode;

    // Constructor that takes a file path
    explicit SimConfig(const std::string& filePath);

    // Static member function for processing a JSON file into a SimConfig object
    static SimConfig processJsonFile(const std::string& filePath);

private:
    // Helper function for mapping strings to ParamKey
    static ParamKey getParamKey(const std::string& key);
};
