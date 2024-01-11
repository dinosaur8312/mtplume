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

void SigmaGrowthPasquillGifford::calcSigmasImpl()
{
    printf("xk_flag, Begin SigmaGrowthPasquillGifford::calcSigmasImp\n");

    for (int sidx = 0; sidx < numst; ++sidx)
    {
        for (int xidx = 0; xidx < numx; ++xidx)
        {
            printf("xk_flag, Begin SigmaGrowthPasquillGifford::calcSigmasImp, xidx = %d, sidx = %d\n", xidx, sidx);
            fx(xidx, sidx);
            fy(xidx, sidx);
            fz(xidx, sidx);
        }
    }
    printf("xk_flag, End SigmaGrowthPasquillGifford::calcSigmasImp\n");
}

void SigmaGrowthPasquillGifford::calcZfunc()
{
    printf("xk_flag, Begin SigmaGrowthPasquillGifford::calcZfunc\n");
    for (int sidx = 0; sidx < numst; ++sidx)
    {
        for (int xidx = 0; xidx < numx; ++xidx)
        {
            zfunc[xidx + sidx * numx] = zFunction(sensor->_h_z[xidx], release->_h_x[0], met->_h_hml[0], sigz[xidx + sidx * numx]);
            printf("xk_flag, calcZfunc, xidx = %d, sidx =%d,sigZ = %f, out=%f\n",
                   xidx, sidx, sigz[xidx + sidx * numx], zfunc[xidx + sidx * numx]);
        }
    }
    printf("xk_flag, End SigmaGrowthPasquillGifford::calcZfunc\n");
}

void SigmaGrowthPasquillGifford::calcDosage()
{
    printf("xk_flag, Begin SigmaGrowthPasquillGifford::calcdosage\n");

    for (int sidx = 0; sidx < numst; ++sidx)
    {
        for (int xidx = 0; xidx < numx; ++xidx)
        {
            for (int tidx = 0; tidx < numt; ++tidx)
            {
                dosage[tidx + xidx * numt + sidx * numt * numx] = insCenterlineDosage(
                    sensor->_h_x[xidx],
                    sensor->_h_t[tidx],
                    release->_h_mass[0],
                    met->_h_speed[0],
                    zfunc[xidx + sidx * numx],
                    sigx[xidx + sidx * numx],
                    sigy[xidx + sidx * numx]);
                printf("xk_flag, calcDosage, tidx = %d, xidx = %d, sidx = %d, out(tidx,xidx,sidx) = %f\n",
                       tidx, xidx, sidx, dosage[tidx + xidx * numt + sidx * numt * numx]);
            }
        }
    }
    printf("xk_flag, End SigmaGrowthPasquillGifford::calcdosage\n");
    return;
}

void SigmaGrowthPasquillGifford::calcHalfWidths()
{
    printf("xk_flag, Begin SigmaGrowthPasquillGifford::calcHalfWidths\n");
    for (int sidx = 0; sidx < numst; ++sidx)
    {
        for (int xidx = 0; xidx < numx; ++xidx)
        {
            for (int tidx = 0; tidx < numt; ++tidx)
            {
                for (int lidx = 0; lidx < numl; ++lidx)
                {
                    hws[lidx + tidx * numl + xidx * numl * numt + sidx * numl * numt * numx] = halfWidth(
                        sensor->_h_level[lidx],
                        dosage[tidx + xidx * numt + sidx * numt * numx],
                        sigy[xidx + sidx * numx]);
                    printf("xk_flag, calcHalfWidths, lidx = %d, tidx = %d, xidx = %d, sidx = %d, out(lidx,tidx,xidx,sidx) = %f\n",
                           lidx, tidx, xidx, sidx, hws[lidx + tidx * numl + xidx * numl * numt + sidx * numl * numt * numx]);
                }
            }
        }
    }
    printf("xk_flag, End SigmaGrowthPasquillGifford::calcHalfWidths\n");
    return;
}

void SigmaGrowthPasquillGifford::updateAreas()
{
    printf("xk_flag, Begin SigmaGrowthPasquillGifford::updateAreas\n");

    for (int sidx = 0; sidx < numst; ++sidx)
    {
        for (int tidx = 0; tidx < numt; ++tidx)
        {
            for (int lidx = 0; lidx < numl; ++lidx)
            {
                float area = 0.f;
                for (int xidx = 0; xidx < numx - 1; ++xidx)
                {
                    area += trapzRule(
                        sensor->_h_x[xidx],
                        sensor->_h_x[xidx+1],
                        hws[lidx+tidx*numl+xidx*numl*numt+sidx*numl*numt*numx],
                        hws[lidx+tidx*numl+(xidx+1)*numl*numt+sidx*numl*numt*numx]);
                }
                areas[lidx + tidx * numl + sidx * numl * numx] = 2.0f * area;
                printf("xk_flag, updateAreas, lidx = %d, tidx = %d, sidx = %d, out(lidx,tidx,sidx) = %f\n",
                       lidx, tidx, sidx, areas[lidx + tidx * numl + sidx * numl * numx]);
            }
        }
    }
    printf("xk_flag, End SigmaGrowthPasquillGifford::updateAreas\n");
}
