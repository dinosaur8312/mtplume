#ifndef MTPLUME_CSVREADER_HPP
#define MTPLUME_CSVREADER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

struct DataRow {
    int id;
    int index;
    double x; // Assuming "X" represents a numeric value
    double sig_x;
    double sig_y;
    double sig_z;
    double D0;
    double D0U_Q;
    double coef_y;
    double expon_y;
    double coef_z;
    double expon_z;
    int iwind; // Iwind as integer type
    int istab; // Istab as integer type
    double wind;
    char stab; // Stab as character type
    std::string how; // How as string type
};

struct DataRow_v1 {
    int id;
    int istab; // Istab as integer type
    double speed;
    char stab; // Stab as character type
    double x; // Assuming "X" represents a numeric value
    double sig_x;
    double sig_y;
    double sig_z;
    double xv_x;
    double xv_y;
    double xv_z;
    double diff_x;
    double diff_y;
    double diff_z;
    std::string pass; // How as string type
};



class DataContainer {
public:
    std::vector<DataRow> rows; // Vector of DataRow instances
};
class DataContainer_v1 {
public:
    std::vector<DataRow_v1> rows; // Vector of DataRow instances
};




// Class to read CSV data and populate DataContainer
class CSVReader {
public:
    // Method to read CSV file and populate data
    DataContainer readCSV(const std::string &fileName) ;
    DataContainer_v1 readCSV_v1(const std::string &fileName) ;
    //DataContainer readCSV(const std::string& fileName, const std::string& indexKey, const std::string& iwindKey, const std::string& istabKey);
};


#endif // MTPLUME_CSVREADER_HPP