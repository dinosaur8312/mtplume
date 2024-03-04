#include "SimConfig.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

// SimConfig constructor implementation
SimConfig::SimConfig(const std::string& filePath) {
    *this = processJsonFile(filePath); // Populate this instance via processJsonFile
}

// Implementation of processJsonFile
SimConfig SimConfig::processJsonFile(const std::string& filePath) {
    SimConfig config;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open JSON input file: " + filePath);
    }

    nlohmann::json j;
    file >> j;
    file.close();

    config.coefPath = j.value("reference", "");
    config.outputPath = j.value("output", "");

    auto& input = j["input"];
    config.inputPath = input.value("filePath", "");

    auto& params = input["parameters"];
    for (auto& [key, value] : params.items()) {
        if (value.value("required", true)) {
            ParamKey paramKey = getParamKey(key); // Make sure to implement getParamKey
            config.parameters[paramKey] = value.value("columnName", "");
        }
    }

    // ComputeMode logic (as previously defined)
    // ...

    return config;
}

ParamKey getParamKey(const std::string& key) {
    if (key == "stability") return ParamKey::Stability;
    else if (key == "windSpeed") return ParamKey::WindSpeed;
    else if (key == "hml") return ParamKey::Hml;
    else if (key == "Q") return ParamKey::Q;
    else if (key == "srcX") return ParamKey::SrcX;
    else if (key == "srcY") return ParamKey::SrcY;
    else if (key == "srcZ") return ParamKey::SrcZ;
    else if (key == "recX") return ParamKey::RecX;
    else if (key == "recY") return ParamKey::RecY;
    else if (key == "recZ") return ParamKey::RecZ;
    else if (key == "sigX") return ParamKey::SigX;
    else if (key == "sigY") return ParamKey::SigY;
    else if (key == "sigZ") return ParamKey::SigZ;
    // Default case or throw an exception if the key is unrecognized
    throw std::invalid_argument("Unrecognized parameter key: " + key);
}

