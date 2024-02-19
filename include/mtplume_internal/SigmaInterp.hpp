#pragma once
#include <math.h>
#include <vector>
#include <iostream>
#include "CSVParser.hpp"
class SigmaInterp
{

public:
    static void compareCSVdata(std::vector<CSVDataRow> coefs, std::vector<CSVDataRow> data)
    {
        printf("xk_flag, Begin compareCSVdata\n");

        // loop over all rows in data
        for (int i = 0; i < data.size(); i++)
        {
            auto row = data[i];
            double x = row.x;
            int istab = row.istab;
            double wind = row.wind;
            double logx = log(x);

            // loop over all rows in coefs
            int id0 = -1;
            int id1 = -1;
            int id2 = -1;
            int id3 = -1;
            bool flag = false;
            for (int j = 1; j < coefs.size(); j++)
            {
                auto row_coef = coefs[j];
                auto row_prev_coef = coefs[j - 1];
                int istab_coef = row_coef.istab;
                //int id = row_coef.id;
                int id = j;
                if (istab_coef != istab)
                    continue;

                double wind_coef = row_coef.wind;
                double x_coef = row_coef.x;
                double x_prev_coef = row_prev_coef.x;
                if ((wind_coef <= wind) && (x >= x_prev_coef) && (x < x_coef))
                {
                    id0 = id - 1;
                    id1 = id;
                    continue;
                }

                if ((id0 >= 0) && (id1 >= 0) && (wind_coef > wind) && (x >= x_prev_coef) && (x < x_coef))
                {
                    id3 = id;
                    id2 = id - 1;
                    flag = true;
                    break;
                }
            }

            if (!flag)
                continue;

            // printf("xk_flag, compareSigmaData, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i,id0, id1, id2, id3);

            auto row0 = coefs[id0];
            auto row1 = coefs[id1];
            auto row2 = coefs[id2];
            auto row3 = coefs[id3];

            double x0 = row0.x;
            double x1 = row1.x;
            double x2 = row2.x;
            double x3 = row3.x;

            double logx0 = log(x0);
            double logx1 = log(x1);
            double logx2 = log(x2);
            double logx3 = log(x3);

            // printf("xk_flag, compareSigmaData,  logx=%f, logx0 = %f, logx1 = %f, logx2 = %f, logx3 = %f\n", logx, logx0, logx1, logx2, logx3);

            double sig_x0 = row0.sig_x;
            double sig_x1 = row1.sig_x;
            double sig_x2 = row2.sig_x;
            double sig_x3 = row3.sig_x;

            double log_sig_x0 = log(sig_x0);
            double log_sig_x1 = log(sig_x1);
            double log_sig_x2 = log(sig_x2);
            double log_sig_x3 = log(sig_x3);

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

            //  printf("xk_flag, compareSigmaData,  sig_x0 = %f, sig_x1 = %f, sig_x2 = %f, sig_x3 = %f\n", sig_x0, sig_x1, sig_x2, sig_x3);

            double wx = (logx - logx0) / (logx1 - logx0);
            // double wx = (x - x0) / (x1 - x0);

            double sig_x01 = (1.f - wx) * sig_x0 + wx * sig_x1;
            double sig_x23 = (1.f - wx) * sig_x2 + wx * sig_x3;

            double log_sig_x01 = (1.f - wx) * log_sig_x0 + wx * log_sig_x1;
            double log_sig_x23 = (1.f - wx) * log_sig_x2 + wx * log_sig_x3;

            double sig_y01 = (1.f - wx) * sig_y0 + wx * sig_y1;
            double sig_y23 = (1.f - wx) * sig_y2 + wx * sig_y3;

            double log_sig_y01 = (1.f - wx) * log_sig_y0 + wx * log_sig_y1;
            double log_sig_y23 = (1.f - wx) * log_sig_y2 + wx * log_sig_y3;

            double sig_z01 = (1.f - wx) * sig_z0 + wx * sig_z1;
            double sig_z23 = (1.f - wx) * sig_z2 + wx * sig_z3;

            //        printf("xk_flag, compareSigmaData,  sig_x01 = %f, sig_x23 = %f\n", sig_x01, sig_x23);

            double w_wind = (wind - row0.wind) / (row2.wind - row0.wind);

            //        printf("xk_flag, compareSigmaData,  wy = %f\n", wy);

            double sig_x_coef = (1.f - w_wind) * sig_x01 + w_wind * sig_x23;
            double sig_y_coef = (1.f - w_wind) * sig_y01 + w_wind * sig_y23;
            double sig_z_coef = (1.f - w_wind) * sig_z01 + w_wind * sig_z23;

            double log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
            double log_sig_y_coef = (1.f - w_wind) * log_sig_y01 + w_wind * log_sig_y23;

            double exp_log_sig_x_coef = exp(log_sig_x_coef);
            double exp_log_sig_y_coef = exp(log_sig_y_coef);

            double sig_x = row.sig_x;
            double sig_y = row.sig_y;
            double sig_z = row.sig_z;

            double error_x = abs(sig_x - sig_x_coef) / abs(sig_x) * 100;
            double error_logx = abs(sig_x - exp_log_sig_x_coef) / abs(sig_x) * 100;
            double error_y = abs(sig_y - sig_y_coef) / abs(sig_y) * 100;
            double error_z = abs(sig_z - sig_z_coef) / abs(sig_z) * 100;

            printf("\ni=%d\n", i);
            printf("id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", id0, id1, id2, id3);
            printf("x = %f, istab = %d, wind = %f \n", x, istab, wind);
            printf("wx = %f, w_wind=%f\n", wx, w_wind);
            printf(" sig_x0 = %f, sig_x1 = %f, sig_x2 = %f, sig_x3 = %f\n", sig_x0, sig_x1, sig_x2, sig_x3);
            printf(" sig_x01 = %f, sig_x23 = %f\n", sig_x01, sig_x23);
            printf(" sig_y0 = %f, sig_y1 = %f, sig_y2 = %f, sig_y3 = %f\n", sig_y0, sig_y1, sig_y2, sig_y3);
            printf(" sig_y01 = %f, sig_y23 = %f\n", sig_y01, sig_y23);
            printf(" sig_z0 = %f, sig_z1 = %f, sig_z2 = %f, sig_z3 = %f\n", sig_z0, sig_z1, sig_z2, sig_z3);
            printf(" sig_z01 = %f, sig_z23 = %f\n", sig_z01, sig_z23);
            printf(" sig_x = %f, sig_x_coef = %f, ERROR_X=%f% \n", sig_x, sig_x_coef, error_x);
            printf(" sig_x = %f, sig_x_coef = %f, ERROR_LOGX=%f% \n", sig_x, exp_log_sig_x_coef, error_logx);
            printf(" sig_y = %f, sig_y_coef = %f, ERROR_Y=%f% \n", sig_y, sig_y_coef, error_y);
            printf(" sig_z = %f, sig_z_coef = %f, ERROR_Z=%f% \n", sig_z, sig_z_coef, error_z);
        }

        printf("xk_flag, ompareCSVdata\n");
    }
};