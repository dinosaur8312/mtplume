#include "SigmaInterp.hpp"
#define EPSILON 0.0001
// #define PRTCHECK

void findFourCoefs(std::vector<CSVDataRow> coefs, const int istab, const float wind, const float x,
                   int &id0, int &id1, int &id2, int &id3, bool &flag)
{
    id0 = -1;
    id1 = -1;
    id2 = -1;
    id3 = -1;
    flag = false;

    std::vector<std::vector<double>> windgap_min(2, std::vector<double>(2, 9999999.f));
    std::vector<std::vector<double>> xgap_min(2, std::vector<double>(2, 9999999.f));

    for (int j = 0; j < coefs.size(); j++)
    {
        auto row_coef = coefs[j];
        int istab_coef = row_coef.istab;
        if (istab_coef != istab)
            continue;

        double wind_coef = row_coef.wind;
        double x_coef = row_coef.x;

        double windgap = abs(wind_coef - wind);
        double xgap = abs(x_coef - x);

        if (wind_coef <= wind)
        {
            if (windgap <= (windgap_min[0][0] + EPSILON))
            {
                if (x_coef <= x)
                {
                    if (xgap <= xgap_min[0][0])
                    {
                        xgap_min[0][0] = xgap;
                        windgap_min[0][0] = windgap;
                        id0 = j;
                    }
                }
            }

            if (windgap <= (windgap_min[0][1] + EPSILON))
            {
                if (x_coef > x)
                {
                    if (xgap <= xgap_min[0][1])
                    {
                        xgap_min[0][1] = xgap;
                        windgap_min[0][1] = windgap;
                        id1 = j;
                    }
                }
            }
        }
        else
        {
            if (windgap <= (windgap_min[1][0] + EPSILON))
            {

                if (x_coef <= x)
                {
                    if (xgap <= xgap_min[1][0])
                    {
                        xgap_min[1][0] = xgap;
                        windgap_min[1][0] = windgap;
                        id2 = j;
                    }
                }
            }

            if (windgap < (windgap_min[1][1] + EPSILON))
            {
                if (x_coef > x)
                {
                    if (xgap <= xgap_min[1][1])
                    {
                        xgap_min[1][1] = xgap;
                        windgap_min[1][1] = windgap;
                        id3 = j;
                    }
                }
            }
        }

        if ((id1 >= 0) && (id2 >= 0) && (id3 >= 0) && (id0 >= 0))
            break;

    } // end of loop over all rows in coefs to find id0, id1, id2, id3

    if ((id0 >= 0) && (id1 == -1))
        id1 = id0;
    if ((id2 >= 0) && (id3 == -1))
        id3 = id2;
    if ((id1 >= 0) && (id0 == -1))
        id0 = id1;
    if ((id3 >= 0) && (id2 == -1))
        id2 = id3;

    if ((id1 >= 0) && (id3 == -1))
    {
        id2 = id0;
        id3 = id1;
    }

    if ((id3 >= 0) && (id1 == -1))
    {
        id1 = id3;
        id0 = id2;
    }
}
void compareSigmaData(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3,
                      const float x, CSVDataRow row,
                      float &sig_x, float &exp_log_sig_x_coef, float &error_logx,
                      const int sigid)
{
    auto row0 = coefs[id0];
    auto row1 = coefs[id1];
    auto row2 = coefs[id2];
    auto row3 = coefs[id3];

    double x0, x1, x2, x3;
    x0 = row0.x;
    x1 = row1.x;
    x2 = row2.x;
    x3 = row3.x;
    double logx0 = log(x0);
    double logx1 = log(x1);
    double logx2 = log(x2);
    double logx3 = log(x3);

    double sig_x0, sig_x1, sig_x2, sig_x3;

    if (sigid == 0)
    {
        sig_x0 = row0.sig_x;
        sig_x1 = row1.sig_x;
        sig_x2 = row2.sig_x;
        sig_x3 = row3.sig_x;
    }
    else if (sigid == 1)
    {
        sig_x0 = row0.sig_y;
        sig_x1 = row1.sig_y;
        sig_x2 = row2.sig_y;
        sig_x3 = row3.sig_y;
    }
    else if (sigid == 2)
    {
        sig_x0 = row0.sig_z;
        sig_x1 = row1.sig_z;
        sig_x2 = row2.sig_z;
        sig_x3 = row3.sig_z;
    }

    double log_sig_x0 = log(sig_x0);
    double log_sig_x1 = log(sig_x1);
    double log_sig_x2 = log(sig_x2);
    double log_sig_x3 = log(sig_x3);

    double logx = log(x);

    double wx01 = id0 == id1 ? 0.f : (logx - logx0) / (logx1 - logx0);
    double wx23 = id2 == id3 ? 0.f : (logx - logx2) / (logx3 - logx2);

    double log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    double log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;

    double w_wind = id0 == id2 ? 0.f : (row.wind - row0.wind) / (row2.wind - row0.wind);

    double log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    exp_log_sig_x_coef = exp(log_sig_x_coef);

#if (PRTCHECK)
    printf("xk_flag, compareSigmaData,  x0 = %f, x1 = %f, x2 = %f, x3 = %f\n", x0, x1, x2, x3);
    printf("xk_flag, compareSigmaData,   logx0 = %f, logx1 = %f, logx2 = %f, logx3 = %f\n", logx0, logx1, logx2, logx3);
    printf("xk_flag, compareSigmaData,  sig_x0 = %f, sig_x1 = %f, sig_x2 = %f, sig_x3 = %f\n", sig_x0, sig_x1, sig_x2, sig_x3);
    printf("xk_flag, compareSigmaData,  log_sig_x0 = %f, log_sig_x1 = %f, log_sig_x2 = %f, log_sig_x3 = %f\n", log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3);
    printf("xk_flag, compareSigmaData,  logx = %f, wx01=%f, wx23=%f\n", logx, wx01, wx23);
    printf("xk_flag, compareSigmaData,  log_sig_x01 = %f, log_sig_x23 = %f\n", log_sig_x01, log_sig_x23);
    printf("xk_flag, compareSigmaData,  w_wind = %f\n", w_wind);
    printf("xk_flag, compareSigmaData,  log_sig_x_coef = %f\n", log_sig_x_coef);
    printf("xk_flag, compareSigmaData,  exp_log_sig_x_coef = %f\n", exp_log_sig_x_coef);
#endif

    if (sigid == 0)
        sig_x = row.sig_x;
    else if (sigid == 1)
        sig_x = row.sig_y;
    else if (sigid == 2)
        sig_x = row.sig_z;

    error_logx = abs(sig_x - exp_log_sig_x_coef) / abs(sig_x) * 100;
}

void compareZfunction(std::vector<CSVDataRow> data, std::ofstream &outputFile)
{

    outputFile << "ID,iplot,zrcp,zplume,hml,sig_z,zFunc_calc,zFunc_ref,error,pass\n";
    int pass_count = 0;
    for (int i = 0; i < data.size(); i++)
    {
        auto row = data[i];
        int iplot = row.istab;
        double zFunc = row.wind;
        double zrcp = row.x;
        double zplume = row.y;
        double hml = row.z;
        double sig_z = row.sig_z;

        float zfunc_calc = zFunction(zrcp, zplume, hml, sig_z);

        float error_zfunc = abs(zFunc)>1E-6? abs(zFunc - zfunc_calc) / abs(zFunc) * 100 : abs(zFunc - zfunc_calc) ;

        outputFile << i << "," << iplot << "," << zrcp << "," << zplume << "," << hml << "," << sig_z << "," << zfunc_calc << "," << zFunc << "," << error_zfunc << ",";

        if (error_zfunc < 0.1f)
        {
            outputFile << "Yes"
                       << "\n";
            pass_count++;
        }
        else
        {
            outputFile << "No"
                       << "\n";
        }
    }
    double pass_rate = (double)pass_count / (double)data.size() * 100;
    // print total number of test cases
    std::cout << "Total number of test cases = " << data.size() << "\n";
    std::cout << "Pass rate = " << pass_rate << "%\n";
}