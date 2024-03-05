#pragma once

#include <string>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp> // Include this for JSON processing

#define MAX_PARAM_NUM 32
enum ParamKeyID
{
    STABILITY_ID = 0,
    WINDSPEED_ID,
    HML_ID,
    Q_ID,
    SRCX_ID,
    SRCY_ID,
    SRCZ_ID,
    RECX_ID,
    RECY_ID,
    RECZ_ID,
    SIGX_ID,
    SIGY_ID,
    SIGZ_ID,
    ZFUNC_ID,
    CONCENTRATION_ID,
    DOSAGE_ID
    // Add other parameters as needed, they will automatically be assigned the next integer values.
};

class SimConfig
{
public:
    std::string coefPath;
    std::string inputPath;
    std::string outputPath;

    void **data_ptr;
    int *header_exist;

    // Constructor that takes a file path
    SimConfig(const std::string &filePath);
    ~SimConfig();

private:
    void processJsonFile(const std::string &filePath);

    std::unordered_map<std::string, ParamKeyID> headerMap = {
        {"stability", STABILITY_ID},
        {"windSpeed", WINDSPEED_ID},
        {"hml", HML_ID},
        {"Q", Q_ID},
        {"srcX", SRCX_ID},
        {"srcY", SRCY_ID},
        {"srcZ", SRCZ_ID},
        {"recX", RECX_ID},
        {"recY", RECY_ID},
        {"recZ", RECZ_ID},
        {"sigX", SIGX_ID},
        {"sigY", SIGY_ID},
        {"sigZ", SIGZ_ID},
        {"zfunc", ZFUNC_ID},
        {"concentration", CONCENTRATION_ID},
        {"dosage", DOSAGE_ID},
    }
};
