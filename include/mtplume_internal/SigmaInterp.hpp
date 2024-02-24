#pragma once
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "CSVParser.hpp"
#include "SimConfig.hpp"
#define PRTCHECK 0
void findFourCoefs(std::vector<CSVDataRow> coefs, const int istab, const float wind, const float x,
                   int &id0, int &id1, int &id2, int &id3, bool &flag);
void compareSigmaData(std::vector<CSVDataRow> coefs, const int id0, const int id1, const int id2, const int id3,
                      const float x, CSVDataRow row,
                      float &sig_x, float &exp_log_sig_x_coef, float &error_logx,
                      const int sigid);
class SigmaInterp
{

public:
    static void compareCSVdata(SimConfig &config)
    {
        #if(PRTCHECK)
        printf("xk_flag, Begin compareCSVdata\n");
        #endif
        // Further processing with config data...
        auto coefs = CSVParser::parseCoefCSV(config.coefCSVPath);

        auto data = CSVParser::parseRefCSV(config.refCSVPath);

        int computeMode;
        if (std::isnan(data[0].y))
        {
            computeMode = 0;
        }
        else
        {
            computeMode = 1;
        }

        std::ofstream outputFile(config.outputCSVPath);
        if (!outputFile)
        {
            throw std::runtime_error("Cannot open the file: " + config.outputCSVPath);
        }

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
        // printf("xmin = %f, xmax = %f, windmin = %f, windmax = %f\n", xmin, xmax, windmin, windmax);

        if (computeMode == 0)
            outputFile << "ID,istab,wind,x,sig_x_calc,sig_y_calc,sig_z_calc,sig_x_ref,sig_y_ref,sig_z_ref,err_x,err_y,err_z,pass\n";
        else
            outputFile << "ID,istab,wind,x,y,z,sig_x_calc,sig_y_calc,sig_z_calc,sig_x_ref,sig_y_ref,sig_z_ref,err_x,err_y,err_z,pass\n";

        int pass_count = 0;
        // loop over all rows in data
        for (int i = 0; i < data.size(); i++)
        {
            auto row = data[i];
            double x = row.x;
            double y, z;
            int istab = row.istab;
            double wind = row.wind;
            outputFile << i << "," << istab << "," << wind << "," << x << ",";

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

            if (computeMode == 1)
            {
                y = row.y;
                z = row.z;
                outputFile << y << "," << z << ",";
                y = y < xmin ? xmin : y;
                y = y > xmax ? xmax : y;
                z = z < xmin ? xmin : z;
                z = z > xmax ? xmax : z;
            }

            int id0, id1, id2, id3;
            bool flag = false;
            float sig_x_ref, sig_y_ref, sig_z_ref;
            float sig_x_cal, sig_y_cal, sig_z_cal;
            float err_x, err_y, err_z;
            findFourCoefs(coefs, istab, wind, x, id0, id1, id2, id3, flag);
#if (PRTCHECK)
            printf("xk_flag, compareSigmaData, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
#endif
            compareSigmaData(coefs, id0, id1, id2, id3, x, row, sig_x_ref, sig_x_cal, err_x, 0);

            if (computeMode == 0)
            {
                compareSigmaData(coefs, id0, id1, id2, id3, x, row, sig_y_ref, sig_y_cal, err_y, 1);
                compareSigmaData(coefs, id0, id1, id2, id3, x, row, sig_z_ref, sig_z_cal, err_z, 2);
            }
            else
            {
                findFourCoefs(coefs, istab, wind, y, id0, id1, id2, id3, flag);
#if (PRTCHECK)
                printf("xk_flag, compareSigmaData, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
#endif
                compareSigmaData(coefs, id0, id1, id2, id3, y, row, sig_y_ref, sig_y_cal, err_y, 1);
                findFourCoefs(coefs, istab, wind, z, id0, id1, id2, id3, flag);
#if (PRTCHECK)
                printf("xk_flag, compareSigmaData, i=%d, id0 = %d, id1 = %d, id2 = %d, id3 = %d\n", i, id0, id1, id2, id3);
#endif
                compareSigmaData(coefs, id0, id1, id2, id3, z, row, sig_z_ref, sig_z_cal, err_z, 2);
            }

#if (PRTCHECK)
            printf(" sig_x = %f, sig_x_coef = %f, ERROR_LOGX=%f% \n", sig_x_ref, sig_x_cal, err_x);
            printf(" sig_y = %f, sig_y_coef = %f, ERROR_LOGY=%f% \n", sig_y_ref, sig_y_cal, err_y);
            printf(" sig_z = %f, sig_z_coef = %f, ERROR_LOGZ=%f% \n", sig_z_ref, sig_z_cal, err_z);
#endif
            outputFile << sig_x_cal << "," << sig_y_cal << "," << sig_z_cal << ",";
            outputFile << sig_x_ref << "," << sig_y_ref << "," << sig_z_ref << ",";
            outputFile << err_x << "," << err_y << "," << err_z << ",";
            if (err_x < 0.1f && err_y < 0.1f && err_z < 0.1f)
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
        } // end of loop over all rows in data
        outputFile.close();
        std::cout << "CSV file has been written successfully.\n";

        double pass_rate = (double)pass_count / (double)data.size() * 100;
        //print total number of test cases
        std::cout << "Total number of test cases = " << data.size() << "\n";
        std::cout << "Pass rate = " << pass_rate << "%\n";

#if (PRTCHECK)
        printf("xk_flag, compareCSVdata\n");
#endif
    }
};