#include "SigmaInterp.hpp"
#define EPSILON 0.0001

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
            if (windgap <= (windgap_min[0][0]+EPSILON))
            {
                if (x_coef <= x)
                {
                    if (xgap <= xgap_min[0][0])
                    {
                        xgap_min[0][0] = xgap;
                        windgap_min[0][0] = windgap;
                        id0 = j;
                       // printf("xk_flag, findFourCoefs,  x_coef = %f, xgap_min[0][0] = %f, windgap_min[0][0]=%f, id0 = %d\n", x_coef, xgap_min[0][0], windgap_min[0][0], id0);
                    }
                }
            }

            if (windgap <= (windgap_min[0][1]+EPSILON))
            {
                if (x_coef > x)
                {
                    if (xgap <= xgap_min[0][1])
                    {
                        xgap_min[0][1] = xgap;
                        windgap_min[0][1] = windgap;
                        id1 = j;
                      //  printf("xk_flag, findFourCoefs,  x_coef = %f, xgap_min[0][1] = %f, windgap_min[0][1]=%f, id1 = %d\n", x_coef, xgap_min[0][1], windgap_min[0][1],id1);
                    }
                }
            }
        }
        else
        {
            if (windgap <= (windgap_min[1][0]+EPSILON))
            {

                // find xlow and xhigh
                if (x_coef <= x)
                {
                    if (xgap <= xgap_min[1][0])
                    {
                        xgap_min[1][0] = xgap;
                        windgap_min[1][0] = windgap;
                        id2 = j;
                       // printf("xk_flag, findFourCoefs,  x_coef = %f, xgap_min[1][0] = %f, id2 = %d\n", x_coef, xgap_min[1][0], id2);
                    }
                }
            }

            if (windgap < (windgap_min[1][1]+EPSILON))
            {

                // find xlow and xhigh
                if (x_coef > x)
                {
                    if (xgap <= xgap_min[1][1])
                    {
                        xgap_min[1][1] = xgap;
                        windgap_min[1][1] = windgap;
                        id3 = j;
                        //printf("xk_flag, findFourCoefs,  x_coef = %f, xgap_min[1][1] = %f, id3 = %d\n", x_coef, xgap_min[1][1], id3);
                    }
                }
            }
        }

        if ((id1 >= 0) && (id2 >= 0) && (id3 >= 0) && (id0 >= 0))
        {
            // flag = true;
            break;
        }

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
        // flag = true;
    }

    if ((id3 >= 0) && (id1 == -1))
    {
        id1 = id3;
        id0 = id2;
        //  flag = true;
    }
}
void findFourCoefs_bk(std::vector<CSVDataRow> coefs, const int istab, const float wind, const float x,
                      int &id0, int &id1, int &id2, int &id3, bool &flag)
{
    id0 = -1;
    id1 = -1;
    id2 = -1;
    id3 = -1;
    flag = false;
    for (int j = 1; j < coefs.size(); j++)
    {
        auto row_coef = coefs[j];
        auto row_prev_coef = coefs[j - 1];
        int istab_coef = row_coef.istab;
        int id = j;
        if (istab_coef != istab)
            continue;

        double wind_coef = row_coef.wind;
        double x_coef = row_coef.x;
        double x_prev_coef = row_prev_coef.x;
        /*
        if (((wind < windmin) || (wind > windmax)) && (x >= x_prev_coef) && (x <= x_coef))
        {
            id0 = id - 1;
            id1 = id;
            id2 = id0;
            id3 = id1;
            flag = true;
            break;
        }
        */
        if ((x >= x_prev_coef) && (x <= x_coef))
        {
            if (wind_coef <= wind)
            {
                id0 = id - 1;
                id1 = id;
            }
            else
            {
                id3 = id;
                id2 = id - 1;
            }
        }

        if ((id1 >= 0) && (id3 >= 0))
        {
            flag = true;
            break;
        }

    } // end of loop over all rows in coefs to find id0, id1, id2, id3

    if ((id1 >= 0) && (id3 == -1))
    {
        id2 = id0;
        id3 = id1;
        flag = true;
    }

    if ((id3 >= 0) && (id1 == -1))
    {
        id1 = id3;
        id0 = id2;
        flag = true;
    }
    if (!flag)
    {
        printf("Didn't found\n");
        /*
        outputFile << "NaN"
                   << ","
                   << "NaN"
                   << ","
                   << "NaN"
                   << ",";
        outputFile << "NaN"
                   << ","
                   << "NaN"
                   << ","
                   << "NaN"
                   << ",";
        outputFile << "NaN"
                   << ","
                   << "NaN"
                   << ","
                   << "NaN"
                   << ",";
        outputFile << "No"
                   << "\n";
                   */
    }
}
void compareSigmaData(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3,
                      const float x, CSVDataRow row,
                      float &sig_x, float &exp_log_sig_x_coef, float &error_logx,
                      const int sigid)
{
    printf("sigid=%d\n", sigid);
    auto row0 = coefs[id0];
    auto row1 = coefs[id1];
    auto row2 = coefs[id2];
    auto row3 = coefs[id3];

    double x0, x1, x2, x3;
    x0 = row0.x;
    x1 = row1.x;
    x2 = row2.x;
    x3 = row3.x;
    printf("xk_flag, compareSigmaData,  x0 = %f, x1 = %f, x2 = %f, x3 = %f\n", x0, x1, x2, x3);

    double logx0 = log(x0);
    double logx1 = log(x1);
    double logx2 = log(x2);
    double logx3 = log(x3);

    printf("xk_flag, compareSigmaData,   logx0 = %f, logx1 = %f, logx2 = %f, logx3 = %f\n", logx0, logx1, logx2, logx3);

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
    printf("xk_flag, compareSigmaData,  sig_x0 = %f, sig_x1 = %f, sig_x2 = %f, sig_x3 = %f\n", sig_x0, sig_x1, sig_x2, sig_x3);

    double log_sig_x0 = log(sig_x0);
    double log_sig_x1 = log(sig_x1);
    double log_sig_x2 = log(sig_x2);
    double log_sig_x3 = log(sig_x3);

    printf("xk_flag, compareSigmaData,  log_sig_x0 = %f, log_sig_x1 = %f, log_sig_x2 = %f, log_sig_x3 = %f\n", log_sig_x0, log_sig_x1, log_sig_x2, log_sig_x3);
    /*
    double sig_y0 = row0.sig_y;
    double sig_y1 = row1.sig_y;
    double sig_y2 = row2.sig_y;
    double sig_y3 = row3.sig_y;

    double log_sig_y0 = log(sig_y0);
    double log_sig_y1 = log(sig_y1);
    double log_sig_y2 = log(sig_y2);
    double log_sig_y3 = log(sig_y3);

    double sig_z0 = row0.sig_z;
    double sig_z1 = row1.sig_z;
    double sig_z2 = row2.sig_z;
    double sig_z3 = row3.sig_z;

    double log_sig_z0 = log(sig_z0);
    double log_sig_z1 = log(sig_z1);
    double log_sig_z2 = log(sig_z2);
    double log_sig_z3 = log(sig_z3);
    */

    //  printf("xk_flag, compareSigmaData,  sig_x0 = %f, sig_x1 = %f, sig_x2 = %f, sig_x3 = %f\n", sig_x0, sig_x1, sig_x2, sig_x3);

    double logx = log(x);

    double wx = id0 == id1 ? 0.f : (logx - logx0) / (logx1 - logx0);
    printf("xk_flag, compareSigmaData,  logx = %f, wx = %f\n", logx, wx);
    // double wx = (x - x0) / (x1 - x0);

    // double sig_x01 = (1.f - wx) * sig_x0 + wx * sig_x1;
    // double sig_x23 = (1.f - wx) * sig_x2 + wx * sig_x3;

    double log_sig_x01 = (1.f - wx) * log_sig_x0 + wx * log_sig_x1;
    double log_sig_x23 = (1.f - wx) * log_sig_x2 + wx * log_sig_x3;
    printf("xk_flag, compareSigmaData,  log_sig_x01 = %f, log_sig_x23 = %f\n", log_sig_x01, log_sig_x23);

    // double sig_y01 = (1.f - wx) * sig_y0 + wx * sig_y1;
    // double sig_y23 = (1.f - wx) * sig_y2 + wx * sig_y3;

    /*
    double log_sig_y01 = (1.f - wx) * log_sig_y0 + wx * log_sig_y1;
    double log_sig_y23 = (1.f - wx) * log_sig_y2 + wx * log_sig_y3;

    double log_sig_z01 = (1.f - wx) * log_sig_z0 + wx * log_sig_z1;
    double log_sig_z23 = (1.f - wx) * log_sig_z2 + wx * log_sig_z3;
    */

    // double sig_z01 = (1.f - wx) * sig_z0 + wx * sig_z1;
    // double sig_z23 = (1.f - wx) * sig_z2 + wx * sig_z3;

    //        printf("xk_flag, compareSigmaData,  sig_x01 = %f, sig_x23 = %f\n", sig_x01, sig_x23);

    double w_wind = id0 == id2 ? 0.f : (row.wind - row0.wind) / (row2.wind - row0.wind);
    printf("xk_flag, compareSigmaData,  w_wind = %f\n", w_wind);
    //        printf("xk_flag, compareSigmaData,  wy = %f\n", wy);

    // double sig_x_coef = (1.f - w_wind) * sig_x01 + w_wind * sig_x23;
    // double sig_y_coef = (1.f - w_wind) * sig_y01 + w_wind * sig_y23;
    // double sig_z_coef = (1.f - w_wind) * sig_z01 + w_wind * sig_z23;

    double log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
    // double log_sig_y_coef = (1.f - w_wind) * log_sig_y01 + w_wind * log_sig_y23;
    // double log_sig_z_coef = (1.f - w_wind) * log_sig_z01 + w_wind * log_sig_z23;
    printf("xk_flag, compareSigmaData,  log_sig_x_coef = %f\n", log_sig_x_coef);
    exp_log_sig_x_coef = exp(log_sig_x_coef);
    printf("xk_flag, compareSigmaData,  exp_log_sig_x_coef = %f\n", exp_log_sig_x_coef);
    // double exp_log_sig_y_coef = exp(log_sig_y_coef);
    // double exp_log_sig_z_coef = exp(log_sig_z_coef);

    if (sigid == 0)
        sig_x = row.sig_x;
    else if (sigid == 1)
        sig_x = row.sig_y;
    else if (sigid == 2)
        sig_x = row.sig_z;
    // double sig_y = row.sig_y;
    // double sig_z = row.sig_z;

    // double error_x = abs(sig_x - sig_x_coef) / abs(sig_x) * 100;
    error_logx = abs(sig_x - exp_log_sig_x_coef) / abs(sig_x) * 100;
    // double error_logy = abs(sig_y - exp_log_sig_y_coef) / abs(sig_y) * 100;
    // double error_logz = abs(sig_z - exp_log_sig_z_coef) / abs(sig_z) * 100;

    // double error_z = abs(sig_z - sig_z_coef) / abs(sig_z) * 100;

    /*
    printf("id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", id0, id1, id2, id3);
    printf("wx = %f, w_wind=%f\n", wx, w_wind);
    printf(" sig_x0 = %f, sig_x1 = %f, sig_x2 = %f, sig_x3 = %f\n", sig_x0, sig_x1, sig_x2, sig_x3);
    printf(" sig_y0 = %f, sig_y1 = %f, sig_y2 = %f, sig_y3 = %f\n", sig_y0, sig_y1, sig_y2, sig_y3);
    printf(" sig_z0 = %f, sig_z1 = %f, sig_z2 = %f, sig_z3 = %f\n", sig_z0, sig_z1, sig_z2, sig_z3);
    printf(" sig_x = %f, sig_x_coef = %f, ERROR_LOGX=%f% \n", sig_x, exp_log_sig_x_coef, error_logx);
    printf(" sig_y = %f, sig_y_coef = %f, ERROR_LOGY=%f% \n", sig_y, exp_log_sig_y_coef, error_logy);
    printf(" sig_z = %f, sig_z_coef = %f, ERROR_LOGZ=%f% \n", sig_z, exp_log_sig_z_coef, error_logz);
    outputFile << exp_log_sig_x_coef << "," << exp_log_sig_y_coef << "," << exp_log_sig_z_coef << ",";
    outputFile << sig_x << "," << sig_y << "," << sig_z << ",";
    outputFile << error_logx << "," << error_logy << "," << error_logz << ",";
    if (error_logx < 1.f && error_logy < 1.f && error_logz < 1.f)
        outputFile << "Yes"
                   << "\n";
    else
        outputFile << "No"
                   << "\n";
*/
}