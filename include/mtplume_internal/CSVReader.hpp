#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

// Class to hold the 3D arrays of data
class DataContainer {
public:
    std::vector<std::vector<std::vector<double>>> sig_x; // 3D array for sig_x
    std::vector<std::vector<std::vector<double>>> sig_y; // 3D array for sig_y
    std::vector<std::vector<std::vector<double>>> sig_z; // 3D array for sig_z
    std::vector<std::vector<std::vector<double>>> D0; // 3D array for D0
    std::vector<std::vector<std::vector<double>>> D0U_Q; // 3D array for D0U/Q
    std::vector<std::vector<std::vector<double>>> coef_y; // 3D array for coef_y
    std::vector<std::vector<std::vector<double>>> expon_y; // 3D array for expon_y
    std::vector<std::vector<std::vector<double>>> coef_z; // 3D array for coef_z
    std::vector<std::vector<std::vector<double>>> expon_z; // 3D array for expon_z

    // Constructor
    DataContainer(int istabSize, int iwindSize, int indexSize);
};

// Class to read CSV data and populate DataContainer
class CSVReader {
public:
    // Method to read CSV file and populate data
    DataContainer readCSV(const std::string& fileName, const std::string& indexKey, const std::string& iwindKey, const std::string& istabKey);
};

