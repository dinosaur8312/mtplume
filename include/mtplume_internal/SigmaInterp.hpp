#pragma once
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "CSVParser.hpp"
#include "SimConfig.hpp"
class SigmaInterp
{

public:
    static void compareCSVdata(SimConfig &config)
    {
        printf("xk_flag, Begin compareCSVdata\n");

        // Further processing with config data...
        auto coefs = CSVParser::parseCoefCSV(config.coefCSVPath);

        auto data = CSVParser::parseRefCSV(config.refCSVPath);

        std::ofstream outputFile(config.outputCSVPath);
        if (!outputFile)
        {
            throw std::runtime_error("Cannot open the file: " + config.outputCSVPath);
        }

        // find min and max wind speed and x
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

        outputFile << "ID,istab,wind,x,sig_x_calc,sig_y_calc,sig_z_calc,sig_x_ref,sig_y,ref,sig_z_ref,err_x,err_y,err_z,pass\n";
        // loop over all rows in data
        for (int i = 0; i < data.size(); i++)
        {
            printf("\ni=%d\n", i);
            auto row = data[i];
            double x = row.x;
            int istab = row.istab;
            double wind = row.wind;
            printf("x = %f, istab = %d, wind = %f \n", x, istab, wind);
            outputFile << i << "," << istab << "," << wind << "," << x << ",";
            if (x < xmin)
                x = xmin;
            if (x > xmax)
                x = xmax;
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
                // int id = row_coef.id;
                int id = j;
                if (istab_coef != istab)
                    continue;

                double wind_coef = row_coef.wind;
                double x_coef = row_coef.x;
                double x_prev_coef = row_prev_coef.x;

                if (((wind < windmin) || (wind > windmax)) && (x >= x_prev_coef) && (x <= x_coef))
                {
                    id0 = id - 1;
                    id1 = id;
                    id2 = id0;
                    id3 = id1;
                    flag = true;
                    break;
                }

                if ((wind_coef <= wind) && (x >= x_prev_coef) && (x <= x_coef))
                {
                    id0 = id - 1;
                    id1 = id;
                    continue;
                }

                if ((id0 >= 0) && (id1 >= 0) && (wind_coef > wind) && (x >= x_prev_coef) && (x <= x_coef))
                {
                    id3 = id;
                    id2 = id - 1;
                    flag = true;
                    break;
                }
            }

            if (!flag)
            {
                printf("Didn't found\n");
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
                continue;
            }

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

            double log_sig_z0 = log(sig_z0);
            double log_sig_z1 = log(sig_z1);
            double log_sig_z2 = log(sig_z2);
            double log_sig_z3 = log(sig_z3);

            //  printf("xk_flag, compareSigmaData,  sig_x0 = %f, sig_x1 = %f, sig_x2 = %f, sig_x3 = %f\n", sig_x0, sig_x1, sig_x2, sig_x3);

            double wx = (logx - logx0) / (logx1 - logx0);
            // double wx = (x - x0) / (x1 - x0);

            // double sig_x01 = (1.f - wx) * sig_x0 + wx * sig_x1;
            // double sig_x23 = (1.f - wx) * sig_x2 + wx * sig_x3;

            double log_sig_x01 = (1.f - wx) * log_sig_x0 + wx * log_sig_x1;
            double log_sig_x23 = (1.f - wx) * log_sig_x2 + wx * log_sig_x3;

            // double sig_y01 = (1.f - wx) * sig_y0 + wx * sig_y1;
            // double sig_y23 = (1.f - wx) * sig_y2 + wx * sig_y3;

            double log_sig_y01 = (1.f - wx) * log_sig_y0 + wx * log_sig_y1;
            double log_sig_y23 = (1.f - wx) * log_sig_y2 + wx * log_sig_y3;

            double log_sig_z01 = (1.f - wx) * log_sig_z0 + wx * log_sig_z1;
            double log_sig_z23 = (1.f - wx) * log_sig_z2 + wx * log_sig_z3;

            // double sig_z01 = (1.f - wx) * sig_z0 + wx * sig_z1;
            // double sig_z23 = (1.f - wx) * sig_z2 + wx * sig_z3;

            //        printf("xk_flag, compareSigmaData,  sig_x01 = %f, sig_x23 = %f\n", sig_x01, sig_x23);

            double w_wind = (wind - row0.wind) / (row2.wind - row0.wind);

            //        printf("xk_flag, compareSigmaData,  wy = %f\n", wy);

            // double sig_x_coef = (1.f - w_wind) * sig_x01 + w_wind * sig_x23;
            // double sig_y_coef = (1.f - w_wind) * sig_y01 + w_wind * sig_y23;
            // double sig_z_coef = (1.f - w_wind) * sig_z01 + w_wind * sig_z23;

            double log_sig_x_coef = (1.f - w_wind) * log_sig_x01 + w_wind * log_sig_x23;
            double log_sig_y_coef = (1.f - w_wind) * log_sig_y01 + w_wind * log_sig_y23;
            double log_sig_z_coef = (1.f - w_wind) * log_sig_z01 + w_wind * log_sig_z23;

            double exp_log_sig_x_coef = exp(log_sig_x_coef);
            double exp_log_sig_y_coef = exp(log_sig_y_coef);
            double exp_log_sig_z_coef = exp(log_sig_z_coef);

            double sig_x = row.sig_x;
            double sig_y = row.sig_y;
            double sig_z = row.sig_z;

            // double error_x = abs(sig_x - sig_x_coef) / abs(sig_x) * 100;
            double error_logx = abs(sig_x - exp_log_sig_x_coef) / abs(sig_x) * 100;
            double error_logy = abs(sig_y - exp_log_sig_y_coef) / abs(sig_y) * 100;
            double error_logz = abs(sig_z - exp_log_sig_z_coef) / abs(sig_z) * 100;

            // double error_z = abs(sig_z - sig_z_coef) / abs(sig_z) * 100;

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
        } // end of loop over all rows in data
        outputFile.close();
        std::cout << "CSV file has been written successfully.\n";

        printf("xk_flag, ompareCSVdata\n");
    }
};