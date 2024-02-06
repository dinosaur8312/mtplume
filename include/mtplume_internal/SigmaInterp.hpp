#ifndef MTPLUME_SIGMAINTERP_HPP
#define MTPLUME_SIGMAINTERP_HPP

#include <math.h>
#include "CSVReader.hpp"
class SigmaInterp
{
private:
    // Public member variables
    DataContainer *_data;

public:
    // Constructor
    SigmaInterp(const std::string& fileName, const std::string& indexKey, const std::string& iwindKey, const std::string& istabKey);

    float getSigmaX(float x, float windSpeed, int stability);
    float getSigmaY(float y, float windSpeed, int stability);
    float getSigmaZ(float z, float windSpeed, int stability);
    // Destructor
    ~SigmaInterp() {}

};

#endif // MTPLUME_SIGMAINTERP_HPP