#include "SimConfig.hpp"
#include "csv.h"
#include <fstream>
#include <nlohmann/json.hpp>

// Utility function to count lines in a file
size_t countLinesInFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    return std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
}

// SimConfig constructor implementation
template<typename T>
SimConfig::SimConfig(const std::string &filePath) : coefPath(""), inputPath(""), outputPath(""), sourceSigma(false),
                                                    istab(nullptr), wind(nullptr), hml(nullptr), x(nullptr), y(nullptr), z(nullptr),
                                                    xv(nullptr), yv(nullptr), zv(nullptr), sig_x(nullptr), sig_y(nullptr), sig_z(nullptr),
                                                    mass(nullptr), zplume(nullptr), zrcp(nullptr), t(nullptr), concentration(nullptr), dosage(nullptr)
{
    processJsonFile(filePath); // Populate this instance via processJsonFile
}

template<typename T>
SimConfig<T>::~SimConfig()
{
    // Clean up dynamically allocated memory if you allocate any in your class
    // For example:
    delete[] istab; // Only if istab is dynamically allocated
    delete[] wind; // Assume dynamic array, so use delete[]
    // Repeat for other dynamically allocated members...
}

// Implementation of processJsonFile
template<typename T>
void SimConfig::processJsonFile(const std::string &filePath)
{

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open JSON input file: " + filePath);
    }

    nlohmann::json j;
    file >> j;
    file.close();

    this->coefPath = j["Reference"]["File"];
    this->outputPath = j["Output"]["File"];
    this->inputPath = j["Input"]["File"];

    // Check if "SourceSigma" key exists and is set to "on"
    auto it = j["Input"].find("SourceSigma");
    if (it != j["Input"].end() && it.value() == "on")
        this->sourceSigma = true;
    else
        this->sourceSigma = false;

    return;
}
