#pragma once

#include <string>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp> // Include this for JSON processing

template<typename T>
class SimConfig
{
public:
    std::string coefPath;
    std::string inputPath;
    std::string outputPath;
    bool sourceSigma;

    int *istab;
    T *wind;
    T *hml;
    T* x; // Assuming "X" represents a numeric value
    T* y;
    T* z;
    T* xv;
    T* yv;
    T* zv;
    T* sig_x;
    T* sig_y;
    T* sig_z;
    T* mass;
    T* hml;
    T* zplume;
    T* zrcp;
    T* t;
    T* concentration;
    T* dosage;

    // Constructor that takes a file path
    SimConfig(const std::string &filePath);
    ~SimConfig();

private:
    void processJsonFile(const std::string &filePath);

};
