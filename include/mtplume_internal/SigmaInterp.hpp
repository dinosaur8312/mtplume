#ifndef MTPLUME_SIGMAINTERP_HPP
#define MTPLUME_SIGMAINTERP_HPP

#include <math.h>
#include "CSVReader.hpp"

/*
class SigmaInterp
{
private:
    // Public member variables
    std::unique_ptr<DataContainer> _data; // Use unique_ptr for automatic memory management

    // Helper function for bilinear interpolation
    float bilinearInterpolate(float x, float y, float fQ11, float fQ21, float fQ12, float fQ22, float x1, float x2, float y1, float y2) {
        float f1 = (x2 - x) / (x2 - x1) * fQ11 + (x - x1) / (x2 - x1) * fQ21;
        float f2 = (x2 - x) / (x2 - x1) * fQ12 + (x - x1) / (x2 - x1) * fQ22;
        return (y2 - y) / (y2 - y1) * f1 + (y - y1) / (y2 - y1) * f2;
    }


public:
    // Constructor

   // Constructor
    SigmaInterp(const std::string& fileName, const std::string& indexKey, const std::string& iwindKey, const std::string& istabKey) {
        CSVReader reader;
        _data = std::make_unique<DataContainer>(reader.readCSV(fileName, indexKey, iwindKey, istabKey));
    }

    float getSigmaX(float x, float windSpeed, int stability);
    float getSigmaY(float x, float windSpeed, int stability);
    float getSigmaZ(float x, float windSpeed, int stability);
    // Destructor
    ~SigmaInterp() {}

};

*/

#endif // MTPLUME_SIGMAINTERP_HPP