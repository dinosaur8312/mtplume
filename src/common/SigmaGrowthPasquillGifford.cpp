#include "SigmaGrowthPasquillGifford.hpp"
#include "CSVReader.hpp"

// Constructor
// SigmaGrowthPasquillGifford::SigmaGrowthPasquillGifford(Sensor *sensor, Release *release, ConstantWindMet *met)
SigmaGrowthPasquillGifford::SigmaGrowthPasquillGifford(Sensor *sensor_ptr, Release *release_ptr, std::unique_ptr<ConstantWindMet> met_ptr)
    : sensor(sensor_ptr), release(release_ptr), met(std::move(met_ptr))
{
    // Perform any necessary initialization here
    printf("xk_flag, Begin SigmaGrowthPasquillGifford::SigmaGrowthPasquillGifford\n");
    numx = sensor->_h_x.size();
    numz = sensor->_h_z.size();
    numt = sensor->_h_t.size();
    numl = sensor->_h_level.size();
    numr = release->_h_mass.size();
    numst = met->_h_stability.size();
    sigx.resize(numx * numst);
    sigy.resize(numx * numst);
    sigz.resize(numx * numst);
    zfunc.resize(numx * numst);
    dosage.resize(numx * numt * numst);
    hws.resize(numx * numt * numl * numst);
    areas.resize(numx * numt * numl * numst);
    calcSigmasImpl();

    if (1)
    {
        compareCSVdata();
    }
    else
    {
        calcZfunc();
        calcDosage();
        calcHalfWidths();
        updateAreas();
    }
}

float SigmaGrowthPasquillGifford ::sigmaFunction(int stability, float dist)
{
    static constexpr float RPD = M_PI / 180.f;
    static constexpr float oneMM = 1.0e-6;
    float tCoef1, tCoef2;
    tCoefs(stability, tCoef1, tCoef2);
    float distKm = fmax(oneMM, fmin(500.f, dist / 1000.f));
    float t = tCoef1 - tCoef2 * log(distKm);
    printf("dist=%f, t=%f\n", dist, t);
    return dist * tan(RPD * t) / 2.15f;
}

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


void SigmaGrowthPasquillGifford::compareSigmaX(DataContainer_v1 data)
{
    printf("xk_flag, Begin SigmaGrowthPasquillGifford::compareSigmaX\n");

    // loop over all rows in data
    for(int i = 0; i < data.rows.size(); i++)
    {
        DataRow_v1 row = data.rows[i];
        float xdw = row.x;
        bool clip = 1;
        double sigx = (clip && xdw < 0.f) ? sigma0 : sigmaFunction(row.istab, xdw);
        double sigx_ref = row.sig_x;
        printf("xk_flag, compareSigmaX, xdw = %f, sigx = %f, sigx_ref = %f\n", xdw, sigx, sigx_ref);

    }



    printf("xk_flag, End SigmaGrowthPasquillGifford::compareSigmaX\n");
}
void SigmaGrowthPasquillGifford::compareSigmaData(DataContainer coefs, DataContainer_v1 data)
{
    printf("xk_flag, Begin SigmaGrowthPasquillGifford::compareSigmaData\n");

    // loop over all rows in data
    for(int i = 0; i < data.rows.size(); i++)
    {
        DataRow_v1 row = data.rows[i];
        double x = row.x;
        int istab = row.istab;
        double wind = row.speed;
        double logx = log(x);

        // loop over all rows in coefs
        int id0=-1;
        int id1=-1;
        int id2=-1;
        int id3=-1;
        bool flag = false;
        for(int j = 1; j < coefs.rows.size(); j++)
        {
            DataRow row_coef = coefs.rows[j];
            DataRow row_prev_coef = coefs.rows[j-1];
            int istab_coef = row_coef.istab;
            int id = row_coef.id;
            if(istab_coef != istab)
                continue;
            
            double wind_coef = row_coef.wind;
            double x_coef = row_coef.x;
            double x_prev_coef = row_prev_coef.x;
            if((wind_coef<=wind)&&(x>=x_prev_coef)&&(x<x_coef))
            {
                id0 = id-1;
                id1 = id;
                continue;
            }

            if((id0>=0)&&(id1>=0)&&(wind_coef>wind)&&(x>=x_prev_coef)&&(x<x_coef))
            {
                id3 = id;
                id2 = id-1;
                flag = true;
                break;
            }

        }

        if(!flag) continue;

        printf("xk_flag, compareSigmaData, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i,id0, id1, id2, id3);


        DataRow row0 = coefs.rows[id0];
        DataRow row1 = coefs.rows[id1];
        DataRow row2 = coefs.rows[id2];
        DataRow row3 = coefs.rows[id3];

        double x0 = row0.x;
        double x1 = row1.x;
        double x2 = row2.x;
        double x3 = row3.x;

        double logx0 = log(x0);
        double logx1 = log(x1);
        double logx2 = log(x2);
        double logx3 = log(x3);

        printf("xk_flag, compareSigmaData,  logx=%f, logx0 = %f, logx1 = %f, logx2 = %f, logx3 = %f\n", logx, logx0, logx1, logx2, logx3);

        double sig_x0 = row0.sig_x;
        double sig_x1 = row1.sig_x;
        double sig_x2 = row2.sig_x;
        double sig_x3 = row3.sig_x;

        printf("xk_flag, compareSigmaData,  sig_x0 = %f, sig_x1 = %f, sig_x2 = %f, sig_x3 = %f\n", sig_x0, sig_x1, sig_x2, sig_x3);

        double wx = (logx - logx0) / (logx1 - logx0);

        printf("xk_flag, compareSigmaData,  wx = %f\n", wx);

        double sig_x01 = (1.f-wx)*sig_x0 + wx*sig_x1; 
        double sig_x23 = (1.f-wx)*sig_x2 + wx*sig_x3;

        printf("xk_flag, compareSigmaData,  sig_x01 = %f, sig_x23 = %f\n", sig_x01, sig_x23);

        double wy = (wind - row0.wind) / (row2.wind - row0.wind);

        printf("xk_flag, compareSigmaData,  wy = %f\n", wy);

        double sig_x_coef = (1.f-wy)*sig_x01 + wy*sig_x23; 

        double sig_x = row.sig_x;

        printf("xk_flag, compareSigmaData, i=%d, x = %f, istab = %d, wind = %f, sig_x = %f, sig_x_coef = %f\n", i, x, istab, wind, sig_x, sig_x_coef);            

    }

    printf("xk_flag, End SigmaGrowthPasquillGifford::compareSigmaData\n");
} 

void SigmaGrowthPasquillGifford::compareCSVdata()
{
    printf("xk_flag, Begin SigmaGrowthPasquillGifford::compareCSVdata\n");

    CSVReader *myreader = new CSVReader();
    DataContainer coefs = myreader->readCSV("/home/xianlong/Code/mtplume/tests/unit_tests/hpac_dispersion_coefs.csv");
    DataContainer_v1 data = myreader->readCSV_v1("/home/xianlong/Code/mtplume/tests/unit_tests/test_virtual_sources_01.csv");
    compareSigmaData(coefs,data);

    delete myreader;
    printf("xk_flag, End SigmaGrowthPasquillGifford::compareCSVdata\n");
}

void SigmaGrowthPasquillGifford::calcZfunc()
{
    printf("xk_flag, Begin SigmaGrowthPasquillGifford::calcZfunc\n");
    for (int sidx = 0; sidx < numst; ++sidx)
    {
        for (int xidx = 0; xidx < numx; ++xidx)
        {
            zfunc[xidx + sidx * numx] = zFunction(sensor->_h_z[xidx], release->_h_loc[0].z, met->_h_hml[0], sigz[xidx + sidx * numx]);
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
                        sensor->_h_x[xidx + 1],
                        hws[lidx + tidx * numl + xidx * numl * numt + sidx * numl * numt * numx],
                        hws[lidx + tidx * numl + (xidx + 1) * numl * numt + sidx * numl * numt * numx]);
                }
                areas[lidx + tidx * numl + sidx * numl * numx] = 2.0f * area;
                printf("xk_flag, updateAreas, lidx = %d, tidx = %d, sidx = %d, out(lidx,tidx,sidx) = %f\n",
                       lidx, tidx, sidx, areas[lidx + tidx * numl + sidx * numl * numx]);
            }
        }
    }
    printf("xk_flag, End SigmaGrowthPasquillGifford::updateAreas\n");
}
