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

void findFourCoefs_sig(std::vector<CSVDataRow> coefs, const int istab, const float wind, const float x,
                   int &id0, int &id1, int &id2, int &id3, bool &flag, const int sigid)
{
    id0 = -1;
    id1 = -1;
    id2 = -1;
    id3 = -1;
    flag = false;

    std::vector<std::vector<double>> windgap_min(2, std::vector<double>(2, 9999999.f));
    std::vector<std::vector<double>> xgap_min(2, std::vector<double>(2, 9999999.f));

    int jminwind=-1;
    int jmaxwind=-1;
    for (int j = 0; j < coefs.size(); j++)
    {
        auto row_coef = coefs[j];
        int istab_coef = row_coef.istab;
        if (istab_coef != istab)
            continue;

        double wind_coef = row_coef.wind;

        double x_coef;
        if(sigid==0)
            x_coef = row_coef.sig_x;
        else if(sigid==1)
            x_coef = row_coef.sig_y;
        else if(sigid==2)
            x_coef = row_coef.sig_z;

        double windgap = abs(wind_coef - wind);
        if (wind_coef <= wind)
        {
            if (windgap < windgap_min[0][0])
            {
                jminwind=j;
                windgap_min[0][0]=windgap;
            }
        }
        else
        {
            if (windgap <= windgap_min[0][1])
            {
                jmaxwind=j;
                windgap_min[0][1]=windgap;
            }

        }

    }
    printf("jminwind=%d\n",jminwind);
    printf("jmaxwind=%d\n",jmaxwind);

    for (int j = jminwind; j < jmaxwind; j++)
    {
        auto row_coef = coefs[j];
        int istab_coef = row_coef.istab;
        if (istab_coef != istab)
            continue;

        double wind_coef = row_coef.wind;

        double x_coef;
        if(sigid==0)
            x_coef = row_coef.sig_x;
        else if(sigid==1)
            x_coef = row_coef.sig_y;
        else if(sigid==2)
            x_coef = row_coef.sig_z;

        double windgap = abs(wind_coef - wind);
        double xgap = abs(x_coef - x);

        if((sigid==2)&&(j>168)&&(j<172))
        {
        printf("\nj=%d\n",j);
        printf("x=%f\n",x);

        printf("wind_coef=%f\n",wind_coef);
        printf("x_coef=%f\n",x_coef);
        printf("windgap=%f\n",windgap);
        printf("xgap=%f\n",xgap);
        }

        if (wind_coef <= wind)
        {
            if((sigid==2)&&(j>168)&&(j<172))
            {
                printf("flag wind0\n");
            }
            if (windgap <= (windgap_min[0][0] + EPSILON))
            {
                if((sigid==2)&&(j>168)&&(j<172))
                {
                    printf("thresh wind0\n");
                }
                if (x_coef <= x)
                {
                    if (xgap <= (xgap_min[0][0]+EPSILON))
                    {
                        xgap_min[0][0] = xgap;
                        windgap_min[0][0] = windgap;
                        id0 = j;
                    //    printf("XXXXXXXXX FOUND id0\n");
                    }
                }
            }

            if (windgap <= (windgap_min[0][1] + EPSILON))
            {
                if((sigid==2)&&(j>168)&&(j<172))
                {
                    printf("thresh wind1\n");
                }
                if (x_coef > x)
                {
                    if (xgap <= (xgap_min[0][1] + EPSILON))
                    {
                        xgap_min[0][1] = xgap;
                        windgap_min[0][1] = windgap;
                        id1 = j;
                        //printf("XXXXXXXXX FOUND id1\n");
                        //printf("xgap_min01=%f,windgap_min01=%f,id1=%d\n",xgap,windgap,j);
                    }
                }
            }
        }
        else
        {
            if((sigid==2)&&(j>168)&&(j<172))
            {
                printf("flag wind1\n");
            }
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

       // if ((id1 >= 0) && (id2 >= 0) && (id3 >= 0) && (id0 >= 0))
         //   break;

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

    if(id1<id0) id1=id0+1;
    if(id3<id2) id3=id2+1;
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

        float error_zfunc = abs(zFunc) > 1E-6 ? abs(zFunc - zfunc_calc) / abs(zFunc) * 100 : abs(zFunc - zfunc_calc);

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
void compareXData(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3,
                      const float sig_x, CSVDataRow row,
                      float &x, float &x_coef, float &error_x,
                      const int sigid)
{
    auto row0 = coefs[id0];
    auto row1 = coefs[id1];
    auto row2 = coefs[id2];
    auto row3 = coefs[id3];

    double x0, x1, x2, x3;
    x0 = row0.sig_x;
    x1 = row1.sig_x;
    x2 = row2.sig_x;
    x3 = row3.sig_x;
    double logx0 = log(x0);
    double logx1 = log(x1);
    double logx2 = log(x2);
    double logx3 = log(x3);

    double sig_x0, sig_x1, sig_x2, sig_x3;

    if (sigid == 0)
    {
        sig_x0 = row0.x;
        sig_x1 = row1.x;
        sig_x2 = row2.x;
        sig_x3 = row3.x;
    }
    else if (sigid == 1)
    {
        sig_x0 = row0.y;
        sig_x1 = row1.y;
        sig_x2 = row2.y;
        sig_x3 = row3.y;
    }
    else if (sigid == 2)
    {
        sig_x0 = row0.z;
        sig_x1 = row1.z;
        sig_x2 = row2.z;
        sig_x3 = row3.z;
    }

    double log_sig_x0 = log(sig_x0);
    double log_sig_x1 = log(sig_x1);
    double log_sig_x2 = log(sig_x2);
    double log_sig_x3 = log(sig_x3);

    double logx = log(sig_x);

    double wx01 = id0 == id1 ? 0.f : (logx - logx0) / (logx1 - logx0);
    double wx23 = id2 == id3 ? 0.f : (logx - logx2) / (logx3 - logx2);

    double log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    double log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;

    double w_wind = id0 == id2 ? 0.f : (row.wind - row0.wind) / (row2.wind - row0.wind);

    double log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    x_coef = exp(log_sig_x_coef);

#if (PRTCHECK)
    printf("xk_flag, compareSigmaData,  x0 = %f, x1 = %f, x2 = %f, x3 = %f\n", x0, x1, x2, x3);
    printf("xk_flag, compareSigmaData,   logx0 = %f, logx1 = %f, logx2 = %f, logx3 = %f\n", logx0, logx1, logx2, logx3);
    printf("xk_flag, compareSigmaData,  sig_x0 = %f, sig_x1 = %f, sig_x2 = %f, sig_x3 = %f\n", sig_x0, sig_x1, sig_x2, sig_x3);
    printf("xk_flag, compareSigmaData,  log_sig_x0 = %f, log_sig_x1 = %f, log_sig_x2 = %f, log_sig_x3 = %f\n", log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3);
    printf("xk_flag, compareSigmaData,  logx = %f, wx01=%f, wx23=%f\n", logx, wx01, wx23);
    printf("xk_flag, compareSigmaData,  log_sig_x01 = %f, log_sig_x23 = %f\n", log_sig_x01, log_sig_x23);
    printf("xk_flag, compareSigmaData,  w_wind = %f\n", w_wind);
    printf("xk_flag, compareSigmaData,  log_sig_x_coef = %f\n", log_sig_x_coef);
    printf("xk_flag, compareSigmaData,  x_coef = %f\n", x_coef);
#endif

    if (sigid == 0)
        x = row.x;
    else if (sigid == 1)
        x = row.y;
    else if (sigid == 2)
        x = row.z;

    error_x = abs(x - x_coef) / abs(x) * 100;
}

void calcData(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3, double x,
              CSVDataRow &row)
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

    double logx = log(x);

    double wx01 = id0 == id1 ? 0.f : (logx - logx0) / (logx1 - logx0);
    double wx23 = id2 == id3 ? 0.f : (logx - logx2) / (logx3 - logx2);

    double w_wind = id0 == id2 ? 0.f : (row.wind - row0.wind) / (row2.wind - row0.wind);

    double sig_x0, sig_x1, sig_x2, sig_x3;
    double log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3;
    double log_sig_x01, log_sig_x23;
    double log_sig_x_coef;

    sig_x0 = row0.sig_x;
    sig_x1 = row1.sig_x;
    sig_x2 = row2.sig_x;
    sig_x3 = row3.sig_x;

    log_sig_x0 = log(sig_x0);
    log_sig_x1 = log(sig_x1);
    log_sig_x2 = log(sig_x2);
    log_sig_x3 = log(sig_x3);

    log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;

    log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    row.sig_x = exp(log_sig_x_coef);

    sig_x0 = row0.sig_y;
    sig_x1 = row1.sig_y;
    sig_x2 = row2.sig_y;
    sig_x3 = row3.sig_y;

    log_sig_x0 = log(sig_x0);
    log_sig_x1 = log(sig_x1);
    log_sig_x2 = log(sig_x2);
    log_sig_x3 = log(sig_x3);

    log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;

    log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    row.sig_y = exp(log_sig_x_coef);

    sig_x0 = row0.sig_z;
    sig_x1 = row1.sig_z;
    sig_x2 = row2.sig_z;
    sig_x3 = row3.sig_z;

    log_sig_x0 = log(sig_x0);
    log_sig_x1 = log(sig_x1);
    log_sig_x2 = log(sig_x2);
    log_sig_x3 = log(sig_x3);

    log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;

    log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    row.sig_z = exp(log_sig_x_coef);
}

void calcSigma(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3, double x,
              CSVDataRow &row, const int comp)
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

    double logx = log(x);

    double wx01 = id0 == id1 ? 0.f : (logx - logx0) / (logx1 - logx0);
    double wx23 = id2 == id3 ? 0.f : (logx - logx2) / (logx3 - logx2);

    printf("x0=%f,x1=%f,x2=%f,x3=%f\n",x0,x1,x2,x3);
    printf("wx01=%f\n",wx01);
    printf("wx23=%f\n",wx23);

    double w_wind = id0 == id2 ? 0.f : (row.wind - row0.wind) / (row2.wind - row0.wind);

    double sig_x0, sig_x1, sig_x2, sig_x3;
    double log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3;
    double log_sig_x01, log_sig_x23;
    double log_sig_x_coef;

    if(comp==0)
    {
    sig_x0 = row0.sig_x;
    sig_x1 = row1.sig_x;
    sig_x2 = row2.sig_x;
    sig_x3 = row3.sig_x;
    }
    else if(comp==1)
    {
    sig_x0 = row0.sig_y;
    sig_x1 = row1.sig_y;
    sig_x2 = row2.sig_y;
    sig_x3 = row3.sig_y;
    printf("sx0=%f,sx1=%f,sx2=%f,sx3=%f\n",sig_x0,sig_x1,sig_x2,sig_x3);
    }
    else if(comp==2)
    {
    sig_x0 = row0.sig_z;
    sig_x1 = row1.sig_z;
    sig_x2 = row2.sig_z;
    sig_x3 = row3.sig_z;
    }


    log_sig_x0 = log(sig_x0);
    log_sig_x1 = log(sig_x1);
    log_sig_x2 = log(sig_x2);
    log_sig_x3 = log(sig_x3);

    log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;
    printf("log_sig_x0=%f, log_sig_x1=%f,log_sig_x2=%f,log_sig_x3=%f\n", log_sig_x0,log_sig_x1,log_sig_x2,log_sig_x3);
    printf("log_sig_x01=%f, log_sig_x23=%f\n", log_sig_x01,log_sig_x23);


    log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    if(comp==0)
    row.sig_x = exp(log_sig_x_coef);
    if (comp==1)
    {
    row.sig_y = exp(log_sig_x_coef);
    printf("sig_y=%f\n",row.sig_y);
    }
    if (comp==2)
    row.sig_z = exp(log_sig_x_coef);

}
void calcData_virtual(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3, double x,
              CSVDataRow &row, const int xid)
{
    auto row0 = coefs[id0];
    auto row1 = coefs[id1];
    auto row2 = coefs[id2];
    auto row3 = coefs[id3];

    double x0, x1, x2, x3;
    if(xid==0)
    {
    x0 = row0.sig_x;
    x1 = row1.sig_x;
    x2 = row2.sig_x;
    x3 = row3.sig_x;
    }
    if(xid==1)
    {
    x0 = row0.sig_y;
    x1 = row1.sig_y;
    x2 = row2.sig_y;
    x3 = row3.sig_y;
    }
    if(xid==2)
    {
    x0 = row0.sig_z;
    x1 = row1.sig_z;
    x2 = row2.sig_z;
    x3 = row3.sig_z;
    }
    double logx0 = log(x0);
    double logx1 = log(x1);
    double logx2 = log(x2);
    double logx3 = log(x3);

    double logx = log(x);

    double wx01 = id0 == id1 ? 0.f : (logx - logx0) / (logx1 - logx0);
    double wx23 = id2 == id3 ? 0.f : (logx - logx2) / (logx3 - logx2);

    double w_wind = id0 == id2 ? 0.f : (row.wind - row0.wind) / (row2.wind - row0.wind);

    double sig_x0, sig_x1, sig_x2, sig_x3;
    double log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3;
    double log_sig_x01, log_sig_x23;
    double log_sig_x_coef;

    sig_x0 = row0.x;
    sig_x1 = row1.x;
    sig_x2 = row2.x;
    sig_x3 = row3.x;

    log_sig_x0 = log(sig_x0);
    log_sig_x1 = log(sig_x1);
    log_sig_x2 = log(sig_x2);
    log_sig_x3 = log(sig_x3);

    log_sig_x01 = (1.f - wx01) * log_sig_x0 + wx01 * log_sig_x1;
    log_sig_x23 = (1.f - wx23) * log_sig_x2 + wx23 * log_sig_x3;

    log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    if (xid == 0)
        row.xv = exp(log_sig_x_coef);
    else if (xid == 1)
        row.yv = exp(log_sig_x_coef);
    else if (xid == 2)
        row.zv = exp(log_sig_x_coef);

}

inline double normcdf(double x)
{
    static constexpr double INV_SQRT2 = 0.7071067811865475;
    return 0.5 * (1. + erf(INV_SQRT2 * x));
}


inline double normpdf(double x)
{
    static constexpr double INV_SQRT2PI = 0.3989422804014327;
    return INV_SQRT2PI * exp(-0.5f * x * x);
}

double centerlineDosage(
    double  x, double  t, double Q, double U, double zFunc, double sigX, double sigY)
{
    if (t < 0.f)
    {
        return 0.f;
    }
    static constexpr float INV_ROOT2PI = 0.3989422804014327;
    float coef = INV_ROOT2PI / (sigY * U) * zFunc;
    return Q * coef * normcdf((U * t - x) / sigX) - normcdf(-x / sigX);
}

double centerlineConcentration(
    double  x, double  t, double Q, double U, double zFunc, double sigX, double sigY)
{
    if (t < 0.f)
    {
        return 0.f;
    }
    static constexpr float INV_ROOT2PI = 0.3989422804014327;
    float coef = INV_ROOT2PI / (sigY * sigX) * zFunc;
    return Q * coef * normpdf((U * t - x) / sigX);
}

void generateSample(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs, std::ofstream &outputFile)
{
    // find min and max wind speed and x in coefs table
    double xmin = 9999999.f;
    double xmax = -9999999.f;
    double windmin = 9999999.f;
    double windmax = -9999999.f;

    for (int i = 0; i < coefs.size(); i++)
    {
        auto row = coefs[i];
        double x = row.x;
        double wind = row.wind;
        if (x < xmin)
            xmin = x;
        if (x > xmax)
            xmax = x;
        if (wind < windmin)
            windmin = wind;
        if (wind > windmax)
            windmax = wind;
    }

    outputFile << "ID,istab,wind,x,y,z,icurve,t,sig_x,sig_y,sig_z, zfunc, cpeak, concentration, dinf, dose\n";

    int pass_count = 0;
    // loop over all rows in data
    for (int i = 0; i < data.size(); i++)
    {
        auto row = data[i];
        double x = row.x;
        double y, z;
        int istab = row.istab;
        double wind = row.wind;

// printf(" istab = %d, wind = %f \n", istab, wind);
// print all row data
#if (PRTCHECK)
        printf("\ni=%d\n", i);
        printf("row.istab: %d, row.wind: %f, row.x: %f, row.y: %f, row.z: %f, row.sig_x: %f, row.sig_y: %f, row.sig_z: %f\n", row.istab, row.wind, row.x, row.y, row.z, row.sig_x, row.sig_y, row.sig_z);
#endif
        if (x < xmin)
            x = xmin;
        if (x > xmax)
            x = xmax;
        // double logx = log(x);
        // double logy, logz;

        int id0, id1, id2, id3;
        bool flag = false;
        findFourCoefs(coefs, istab, wind, x, id0, id1, id2, id3, flag);
#if (PRTCHECK)
        printf("xk_flag, compareSigmaData, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
#endif

        calcData(coefs, id0, id1, id2, id3, x, row);

        double zfunc = zFunction(row.zrcp, row.zplume, row.hml, row.sig_z);

        double concentration = centerlineConcentration(
            row.x, row.t, row.mass, row.wind, zfunc, row.sig_x, row.sig_y);
        double dosage = centerlineDosage(
            row.x, row.t, row.mass, row.wind, zfunc, row.sig_x, row.sig_y);

        double defaultnan = -999.;
        outputFile << i << "," << istab << "," << wind << ",";
        outputFile << row.x << "," << row.y << "," << row.z << ",";
        outputFile << row.icurve<<","<<row.t<< ",";
        outputFile << row.sig_x << "," << row.sig_y << "," << row.sig_z << ",";
        outputFile << zfunc << "," << defaultnan << "," << concentration <<","<< defaultnan<< ","<< dosage << "\n";

    } // end of loop over all rows in data
    outputFile.close();
    std::cout << "CSV file has been written successfully.\n";

    double pass_rate = (double)pass_count / (double)data.size() * 100;
    // print total number of test cases
    std::cout << "Total number of test cases = " << data.size() << "\n";
    std::cout << "Pass rate = " << pass_rate << "%\n";
}
void generateSourceSigma(std::vector<CSVDataRow> data, std::vector<CSVDataRow> coefs, std::ofstream &outputFile)
{
    // find min and max wind speed and x in coefs table
    double xmin = 9999999.f;
    double xmax = -9999999.f;
    double ymin = 9999999.f;
    double ymax = -9999999.f;
    double zmin = 9999999.f;
    double zmax = -9999999.f;

    double windmin = 9999999.f;
    double windmax = -9999999.f;

    for (int i = 0; i < coefs.size(); i++)
    {
        auto row = coefs[i];
        double x = row.sig_x;
        double y = row.sig_y;
        double z = row.sig_z;
        double wind = row.wind;
        if (x < xmin)
            xmin = x;
        if (x > xmax)
            xmax = x;
        if (y < ymin)
            ymin = y;
        if (y > ymax)
            ymax = y;
        if (z < zmin)
            zmin = z;
        if (z > zmax)
            zmax = z;
        
        if (wind < windmin)
            windmin = wind;
        if (wind > windmax)
            windmax = wind;
    }

    outputFile << "ID,istab,wind,x,y,z,icurve,t,sig_x,sig_y,sig_z, zfunc, cpeak, concentration, dinf, dose\n";

    int pass_count = 0;
    // loop over all rows in data
    for (int i = 0; i < data.size(); i++)
    {
        auto row = data[i];
        double x = row.sig_x;
        double y = row.sig_y;
        double z = row.sig_z;
        int istab = row.istab;
        double wind = row.wind;

// printf(" istab = %d, wind = %f \n", istab, wind);
// print all row data
#if (PRTCHECK)
        printf("\ni=%d\n", i);
        printf("row.istab: %d, row.wind: %f, row.x: %f, row.y: %f, row.z: %f, row.sig_x: %f, row.sig_y: %f, row.sig_z: %f\n", row.istab, row.wind, row.x, row.y, row.z, row.sig_x, row.sig_y, row.sig_z);
#endif
        if (x < xmin)
            x = xmin;
        if (x > xmax)
            x = xmax;
        if (y < ymin)
            y = ymin;
        if (y > ymax)
            y = ymax;
        if (z < zmin)
            z = zmin;
        if (z > zmax)
            z = zmax;
        printf("clip sig_x = %f\n",x);
        printf("clip sig_y = %f\n",y);
        printf("clip sig_z = %f\n",z);
        // double logx = log(x);
        // double logy, logz;

        int id0, id1, id2, id3;
        bool flag = false;
        findFourCoefs_sig(coefs, istab, wind, x, id0, id1, id2, id3, flag, 0);
#if (PRTCHECK)
        printf("xk_flag, compareSigmaData:sigmax search, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
#endif

        calcData_virtual(coefs, id0, id1, id2, id3, x, row,0);
        printf("xk_flag, compareSigmaData, row.xv = %f\n", row.xv);

        findFourCoefs_sig(coefs, istab, wind, y, id0, id1, id2, id3, flag, 1);
        printf("xk_flag, compareSigmaData:sigmay search, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
        calcData_virtual(coefs, id0, id1, id2, id3, y, row,1);
        printf("xk_flag, compareSigmaData, row.yv = %f\n", row.yv);

        findFourCoefs_sig(coefs, istab, wind, z, id0, id1, id2, id3, flag, 2);
        printf("xk_flag, compareSigmaData:sigmaz search, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
        calcData_virtual(coefs, id0, id1, id2, id3, z, row,2);
#if (PRTCHECK)
        printf("xk_flag, compareSigmaData, row.zv = %f\n", row.zv);
#endif
        printf("xk_flag, compareSigmaData, row.x+row.xv = %f\n", row.x+row.xv);
        findFourCoefs(coefs, istab, wind, row.x+row.xv, id0, id1, id2, id3, flag);
        printf("xk_flag, compareSigmaData: x search: id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", id0, id1, id2, id3);
        calcSigma(coefs, id0, id1, id2, id3, row.x+row.xv, row, 0);


        findFourCoefs(coefs, istab, wind, row.x+row.yv, id0, id1, id2, id3, flag);
        printf("xk_flag, compareSigmaData: y search: id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", id0, id1, id2, id3);
        calcSigma(coefs, id0, id1, id2, id3, row.yv+row.x, row, 1);
        findFourCoefs(coefs, istab, wind, row.x+row.zv, id0, id1, id2, id3, flag);
        printf("xk_flag, compareSigmaData: z search: id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", id0, id1, id2, id3);
        calcSigma(coefs, id0, id1, id2, id3, row.zv+row.x, row, 2);
        printf("xk_flag, compareSigmaData, row.sig_x = %f\n", row.sig_x);
        printf("xk_flag, compareSigmaData, row.sig_y = %f\n", row.sig_y);
        printf("xk_flag, compareSigmaData, row.sig_z = %f\n", row.sig_z);

        //findFourCoefs_sig(coefs, istab, wind, y, id0, id1, id2, id3, flag);
        //calcData_virtual(coefs, id0, id1, id2, id3, y, row,1);
        //findFourCoefs_sig(coefs, istab, wind, z, id0, id1, id2, id3, flag);
        //calcData_virtual(coefs, id0, id1, id2, id3, z, row, 2);

        printf("zrcp=%f,zplume=%f,hml=%f\n",row.zrcp,row.zplume,row.hml);


        double zfunc = zFunction(row.zrcp, row.zplume, row.hml, row.sig_z);
        printf("xk_flag, compareSigmaData, zfunc = %f\n", zfunc);

        double concentration = centerlineConcentration(
            (row.x+row.xv), row.t, row.mass, row.wind, zfunc, row.sig_x, row.sig_y);
        double dosage = centerlineDosage(
            (row.x+row.xv), row.t, row.mass, row.wind, zfunc, row.sig_x, row.sig_y);

        double defaultnan = -999.;
        outputFile << i << "," << istab << "," << wind << ",";
        outputFile << row.x << "," << row.y << "," << row.z << ",";
        outputFile << row.icurve<<","<<row.t<< ",";
        outputFile << row.sig_x << "," << row.sig_y << "," << row.sig_z << ",";
        outputFile << zfunc << "," << defaultnan << "," << concentration <<","<< defaultnan<< ","<< dosage << "\n";

    } // end of loop over all rows in data
    outputFile.close();
    std::cout << "CSV file has been written successfully.\n";

    double pass_rate = (double)pass_count / (double)data.size() * 100;
    // print total number of test cases
    std::cout << "Total number of test cases = " << data.size() << "\n";
    std::cout << "Pass rate = " << pass_rate << "%\n";
}