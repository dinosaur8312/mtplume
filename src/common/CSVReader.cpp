#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "CSVReader.hpp"

// Constructor
DataContainer CSVReader::readCSV(const std::string &fileName) {
    std::ifstream file(fileName);
    std::string line;
    DataContainer container;

    // Skip the header line
    std::getline(file, line);

    // Read each line from the CSV file
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        DataRow row;

        // Parse each column and store it in the DataRow structure
        ss >> row.id; // Directly read the id (first column)
        ss.ignore(1, ','); // Ignore the comma
        ss >> row.index;
        ss.ignore(1, ',');
        ss >> row.x;
        ss.ignore(1, ',');
        ss >> row.D0;
        ss.ignore(1, ',');
        // Continue for other numerical columns
        ss >> row.sig_x;
        ss.ignore(1, ',');
        ss >> row.sig_y;
        ss.ignore(1, ',');
        ss >> row.sig_z;
        ss.ignore(1, ',');
        ss >> row.D0U_Q;
        ss.ignore(1, ',');
        ss >> row.coef_y;
        ss.ignore(1, ',');
        ss >> row.expon_y;
        ss.ignore(1, ',');
        ss >> row.coef_z;
        ss.ignore(1, ',');
        ss >> row.expon_z;
        ss.ignore(1, ',');
        ss >> row.iwind;
        ss.ignore(1, ',');
        ss >> row.istab;
        ss.ignore(1, ',');
        ss >> row.wind;
        ss.ignore(1, ',');
        ss >> row.stab; // Assuming 'stab' is the next value to read
        ss.ignore(1, ',');
        std::getline(ss, row.how); // Read the remainder of the line as "how"

        //print row data
        //std::cout << "id: " << row.id << ", index: " << row.index << ", x: " << row.x << ", sig_x: " << row.sig_x << ", sig_y: " << row.sig_y << ", sig_z: " << row.sig_z << ", D0: " << row.D0 << ", D0U_Q: " << row.D0U_Q << ", coef_y: " << row.coef_y << ", expon_y: " << row.expon_y << ", coef_z: " << row.coef_z << ", expon_z: " << row.expon_z << ", iwind: " << row.iwind << ", istab: " << row.istab << ", wind: " << row.wind << ", stab: " << row.stab << ", how: " << row.how << std::endl;

        // Add the populated DataRow to the DataContainer
        container.rows.push_back(row);
    }

    return container;
}
DataContainer_v1 CSVReader::readCSV_v1(const std::string &fileName) {
    std::ifstream file(fileName);
    std::string line;
    DataContainer_v1 container;

    // Skip the header line
    std::getline(file, line);

    // Read each line from the CSV file
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        DataRow_v1 row;

        // Parse each column and store it in the DataRow structure
        ss >> row.id; // Directly read the id (first column)
        ss.ignore(1, ','); // Ignore the comma
        ss >> row.istab;
        ss.ignore(1, ','); // Ignore the comma
        ss >> row.speed;
        ss.ignore(1, ',');
        ss >> row.stab;
        ss.ignore(1, ',');
        ss >> row.x;
        ss.ignore(1, ',');
        ss >> row.sig_x;
        ss.ignore(1, ',');
        ss >> row.sig_y;
        ss.ignore(1, ',');
        ss >> row.sig_z;
        ss.ignore(1, ',');
        ss >> row.xv_x;
        ss.ignore(1, ',');
        ss >> row.xv_y;
        ss.ignore(1, ',');
        ss >> row.xv_z;
        ss.ignore(1, ',');
        ss >> row.diff_x;
        ss.ignore(1, ',');
        ss >> row.diff_y;
        ss.ignore(1, ',');
        ss >> row.diff_z;
        ss.ignore(1, ',');
        std::getline(ss, row.pass); // Read the remainder of the line 

        //print row data
        std::cout << "id: " << row.id << ", istab: " << row.istab << ", speed: " << row.speed << ", stab: " << row.stab << ", x: " << row.x << ", sig_x: " << row.sig_x << ", sig_y: " << row.sig_y << ", sig_z: " << row.sig_z << ", xv_x: " << row.xv_x << ", xv_y: " << row.xv_y << ", xv_z: " << row.xv_z << ", diff_x: " << row.diff_x << ", diff_y: " << row.diff_y << ", diff_z: " << row.diff_z << ", pass: " << row.pass << std::endl;

        // Add the populated DataRow to the DataContainer
        container.rows.push_back(row);
    }

    return container;
}