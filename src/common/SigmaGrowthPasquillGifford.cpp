#include "SigmaGrowthPasquillGifford.hpp"
void SigmaGrowthPasquillGifford::coefs(int stab, float dist, float &aCoef, float &bCoef)
{
    if (isfinite(dist))
    {
        for (int i = 0; 0 < 10; ++i)
        {
            if (dist <= stabCoefs[stab][i][0])
            {
                aCoef = stabCoefs[stab][i][1];
                bCoef = stabCoefs[stab][i][2];
                return;
            }
        }
    }
    aCoef = std::numeric_limits<float>::quiet_NaN();
    bCoef = std::numeric_limits<float>::quiet_NaN();
}