#include "Release.hpp"
#include "ConstantWindMet.hpp"
#include "Sensors.hpp"
#include <math.h>

class SigmaGrowthPasquillGifford
{
private:
    static constexpr float posInf = std::numeric_limits<float>::infinity();
    static constexpr float stabCoefs[6][10][3] = {
        {// A stability
         {100.0f, 122.80f, 0.9447f},
         {150.0f, 158.08f, 1.0542f},
         {200.0f, 170.22f, 1.0932f},
         {250.0f, 179.52f, 1.1262f},
         {300.0f, 217.41f, 1.2644f},
         {400.0f, 258.89f, 1.4094f},
         {500.0f, 346.75f, 1.7283f},
         {3110.0f, 453.85f, 2.1166f},
         {posInf, 453.85f, 2.1166f},
         {0.0f, 0.0f, 0.0f}},
        {// B stability
         {200.0f, 90.673f, 0.93198f},
         {400.0f, 98.483f, 0.98332f},
         {35000.0f, 109.300f, 1.09710f},
         {posInf, 109.300f, 1.09710f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {// C stability
         {1.0f, 61.141f, 0.91465f},
         {posInf, 61.141f, 0.91465f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {// D stability
         {300.0f, 34.459f, 0.86974f},
         {1000.0f, 32.093f, 0.81066f},
         {3000.0f, 32.093f, 0.64030f},
         {10000.0f, 33.504f, 0.60486f},
         {30000.0f, 36.650f, 0.56589f},
         {posInf, 44.053f, 0.51179f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f}},
        {// E stability
         {100.0f, 24.260f, 0.83660f},
         {300.0f, 23.331f, 0.81956f},
         {1000.0f, 21.628f, 0.75660f},
         {2000.0f, 21.628f, 0.63077f},
         {4000.0f, 22.534f, 0.57154f},
         {10000.0f, 24.703f, 0.50527f},
         {20000.0f, 26.970f, 0.46713f},
         {40000.0f, 35.420f, 0.37615f},
         {posInf, 47.618f, 0.29592f},
         {0.0f, 0.0f, 0.0f}},
        {// F stability
         {200.0f, 15.209f, 0.81558f},
         {700.0f, 14.457f, 0.78407f},
         {1000.0f, 13.953f, 0.68465f},
         {2000.0f, 13.953f, 0.63227f},
         {3000.0f, 14.823f, 0.54503f},
         {7000.0f, 16.187f, 0.46490f},
         {15000.0f, 17.836f, 0.41500f},
         {30000.0f, 22.651f, 0.32681f},
         {60000.0f, 27.074f, 0.27436f},
         {posInf, 34.219f, 0.21716f}}};

    static constexpr float tStabCoefs[6][2] = {
        {24.167f, 2.53340f},
        {18.333f, 1.80960f},
        {12.500f, 1.08570f},
        {8.3333f, 0.72382f},
        {6.2500f, 0.54287f},
        {4.1667f, 0.36191f}};

    void coefs(int stab, float dist, float &aCoef, float &bCoef);

    static void tCoefs(int stab, float &tCoef1, float &tCoef2)
    {
        tCoef1 = tStabCoefs[stab][0];
        tCoef2 = tStabCoefs[stab][1];
    }

public:
    // Constructor
    SigmaGrowthPasquillGifford()
    {
    }

    // Destructor
    ~SigmaGrowthPasquillGifford()
    {
        // Perform any necessary cleanup here
    }

    static float sigmaFunction(int stability, float dist)
    {
        static constexpr float RPD = M_PI / 180.f;
        static constexpr float oneMM = 1.0e-6;
        float tCoef1, tCoef2;
        tCoefs(stability, tCoef1, tCoef2);
        float distKm = fmax(oneMM, fmin(500.f, dist / 1000.f));
        float t = tCoef1 - tCoef2 * log(distKm);
        return dist * tan(RPD * t) / 2.15f;
    }
};