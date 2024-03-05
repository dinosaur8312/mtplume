#include "SimConfig.hpp"
#include "csv.h"
#include <fstream>
#include <nlohmann/json.hpp>



// Utility function to count lines in a file
size_t countLinesInFile(const std::string& filePath) {
    std::ifstream file(filePath);
    return std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
}


// SimConfig constructor implementation
SimConfig::SimConfig(const std::string& filePath) 
{
    this->header_exist = new int[MAX_PARAM_NUM];
    this->data_ptr = new void*[MAX_PARAM_NUM];
    for(int i=0;i<MAX_PARAM_NUM;i++)
    {
        this->header_exist[i]=0;
        this->data_ptr[i] = nullptr;
    }

    processJsonFile(filePath); // Populate this instance via processJsonFile
}

// Implementation of processJsonFile
void SimConfig::processJsonFile(const std::string& filePath) {

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open JSON input file: " + filePath);
    }

    nlohmann::json j;
    file >> j;
    file.close();

    this->coefPath = j["reference"];
    this->outputPath = j["output"];
    this->inputPath = j["input"]["filePath"];

    auto& input_headers = j["input"]["headers"];

    // Count lines in CSV to determine array sizes
    size_t caseNum = countLinesInFile(this->inputPath);


    int header_num=0;
    for (auto& [key, value] : input_headers.items()) {
        if (value=="on") {
            auto found = headerMap.find(key);
            if(found!=headerMap.end())
            {
                this->header_exist[found->second]=1;
                this->data_ptr[found->second] = malloc(sizeof(float)*caseNum);
                header_num++;
            }
        }
    }


    return;
}


