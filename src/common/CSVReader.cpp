#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "CSVReader.hpp"

// Constructor
DataContainer::DataContainer(int istabSize, int iwindSize, int indexSize)
{
    x.resize(istabSize, std::vector<std::vector<double>>(iwindSize, std::vector<double>(indexSize)));
    sig_x.resize(istabSize, std::vector<std::vector<double>>(iwindSize, std::vector<double>(indexSize)));
    sig_y.resize(istabSize, std::vector<std::vector<double>>(iwindSize, std::vector<double>(indexSize)));
    sig_z.resize(istabSize, std::vector<std::vector<double>>(iwindSize, std::vector<double>(indexSize)));
    D0.resize(istabSize, std::vector<std::vector<double>>(iwindSize, std::vector<double>(indexSize)));
    D0U_Q.resize(istabSize, std::vector<std::vector<double>>(iwindSize, std::vector<double>(indexSize)));
    coef_y.resize(istabSize, std::vector<std::vector<double>>(iwindSize, std::vector<double>(indexSize)));
    expon_y.resize(istabSize, std::vector<std::vector<double>>(iwindSize, std::vector<double>(indexSize)));
    coef_z.resize(istabSize, std::vector<std::vector<double>>(iwindSize, std::vector<double>(indexSize)));
    expon_z.resize(istabSize, std::vector<std::vector<double>>(iwindSize, std::vector<double>(indexSize)));
};

// read CSV data and populate DataContainer
DataContainer CSVReader::readCSV(const std::string &fileName, const std::string &indexKey, const std::string &iwindKey, const std::string &istabKey)
{
    std::ifstream file(fileName);
    std::string line;

    // Read header to determine column indices
    std::getline(file, line);
    std::stringstream headerStream(line);
    std::string cell;
    std::unordered_map<std::string, int> columnIndices;
    int columnIndex = 0;
    while (std::getline(headerStream, cell, ','))
    {
        columnIndices[cell] = columnIndex++;
    }

    // Determine the range for each key
    int maxIndex = 0, maxIwind = 0, maxIstab = 0;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        int index, iwind, istab;
        // Extract key values
        for (int i = 0; i <= columnIndices[indexKey]; ++i)
            ss >> index;
        for (int i = columnIndices[indexKey]; i < columnIndices[iwindKey]; ++i)
            ss.ignore(256, ',');
        ss >> iwind;
        for (int i = columnIndices[iwindKey]; i < columnIndices[istabKey]; ++i)
            ss.ignore(256, ',');
        ss >> istab;

        // Update max values
        maxIndex = std::max(maxIndex, index + 1);
        maxIwind = std::max(maxIwind, iwind + 1);
        maxIstab = std::max(maxIstab, istab + 1);
    }

    // Reset file stream to start reading data
    file.clear();
    file.seekg(0);

    // Initialize DataContainer with determined sizes
    DataContainer data(maxIstab, maxIwind, maxIndex);

    // Skip header line
    std::getline(file, line);

    // Read and populate data
    double value;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        int index, iwind, istab;

        // ... (Read index, iwind, istab as before)
        // Read index value
        for (int i = 0; i <= columnIndices[indexKey]; ++i)
        {
            if (i == columnIndices[indexKey])
                ss >> index;
            else
                ss.ignore(256, ',');
        }

        // Read iwind value
        for (int i = columnIndices[indexKey]; i <= columnIndices[iwindKey]; ++i)
        {
            if (i == columnIndices[iwindKey])
                ss >> iwind;
            else
                ss.ignore(256, ',');
        }

        // Read istab value
        for (int i = columnIndices[iwindKey]; i <= columnIndices[istabKey]; ++i)
        {
            if (i == columnIndices[istabKey])
                ss >> istab;
            else
                ss.ignore(256, ',');
        }

        // Read and store data in DataContainer
        for (int i = 0; i < columnIndices.size(); ++i)
        {
            ss >> value;
            if (i == columnIndices["x"])
                data.x[istab][iwind][index] = value;
            if (i == columnIndices["sig_x"])
                data.sig_x[istab][iwind][index] = value;
            if (i == columnIndices["sig_y"])
                data.sig_y[istab][iwind][index] = value;
            if (i == columnIndices["sig_z"])
                data.sig_z[istab][iwind][index] = value;
            if (i == columnIndices["D0"])
                data.D0[istab][iwind][index] = value;
            if (i == columnIndices["D0U_Q"])
                data.D0U_Q[istab][iwind][index] = value;
            if (i == columnIndices["coef_y"])
                data.coef_y[istab][iwind][index] = value;
            if (i == columnIndices["expon_y"])
                data.expon_y[istab][iwind][index] = value;
            if (i == columnIndices["coef_z"])
                data.coef_z[istab][iwind][index] = value;
            if (i == columnIndices["expon_z"])
                data.expon_z[istab][iwind][index] = value;

            // Add conditions for other data fields
        }
    }

    return data;
};

/*
int main() {
    CSVReader reader;
    DataContainer data = reader.readCSV("hpac_dispersion_coefs.csv", "index", "iwind", "istab");
    // Use data as needed
    return 0;
}
*/