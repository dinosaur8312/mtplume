#pragma once

#include "csv.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include "SimConfig.hpp"

struct CSVDataRow
{
    // int id;
    // int index;
    double x; // Assuming "X" represents a numeric value
    double y;
    double z;
    // double D0;
    double sig_x;
    double sig_y;
    double sig_z;
    // double D0U_Q;
    // double coef_y;
    // double expon_y;
    // double coef_z;
    // double expon_z;
    // int iwind; // Iwind as integer type
    int istab; // Istab as integer type
    double wind;
    double mass;
    int icurve;

    double hml;
    double zplume;
    double zrcp;
    double t;
    double concentration;
    double dosage;
    double xv;
    double yv;
    double zv;
    // char stab; // Stab as character type
    // std::string how; // How as string type
};



class CSVParser
{
public:
    static std::vector<CSVDataRow> parseCoefCSV(const std::string &filePath)
    {
        std::vector<CSVDataRow> rows;
        io::CSVReader<6, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
        in.read_header(io::ignore_extra_column, "x", "sig_x", "sig_y", "sig_z", "istab", "wind");
        CSVDataRow row;
        while (in.read_row(row.x, row.sig_x, row.sig_y, row.sig_z, row.istab, row.wind))
        {
            rows.push_back(row);
        }
        return rows;
    }
    static std::vector<CSVDataRow> parseRefCSV(const SimConfig &config)
    {
        std::vector<CSVDataRow> rows;

        // Check if the file name ends with "_02.csv"
        //bool isExtendedFormat = filePath.size() >= 7 && filePath.substr(filePath.size() - 7) == "_02.csv";

        auto filePath = config.refCSVPath;

        //print computeMode
        std::cout << "computeMode: " << config.computeMode << std::endl;

        if (config.computeMode ==1)
        {
            // For *_02.csv files, read 8 columns
            io::CSVReader<8, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "istab", "speed", "x0x", "x0y", "x0z", "sig_x0", "sig_y0", "sig_z0");
            CSVDataRow row;
            while (in.read_row(row.istab, row.wind, row.x, row.y, row.z, row.sig_x, row.sig_y, row.sig_z))
            {
                //printf("row.istab: %d, row.wind: %f, row.x: %f, row.y: %f, row.z: %f, row.sig_x: %f, row.sig_y: %f, row.sig_z: %f\n", row.istab, row.wind, row.x, row.y, row.z, row.sig_x, row.sig_y, row.sig_z);
                rows.push_back(row);
            }
        }
        else if(config.computeMode == 2)
        {
            io::CSVReader<6, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "iplot", "zrcp", "zplume", "hml","sigz", "zfunc");
            CSVDataRow row;
            while (in.read_row(row.istab, row.x, row.y, row.z, row.sig_z, row.wind))
            {
                row.sig_x = std::nan("");
                row.sig_y = std::nan("");
                rows.push_back(row);
            }
        }
        else if(config.computeMode == 3)
        {
            io::CSVReader<10, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "istab","U","zi","Q_mg","x","y","z", "zplume", "icurve","t");
            CSVDataRow row;
            while (in.read_row(row.istab, row.wind, row.hml,row.mass,row.x, row.y, row.zrcp, row.zplume, row.icurve, row.t))
            {
                row.sig_x = std::nan("");
                row.sig_y = std::nan("");
                rows.push_back(row);
            }
        }
        else if(config.computeMode==4)
        {
            io::CSVReader<12, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "istab","U","zi","Q_mg","x","sig_x0","sig_y0","sig_z0", "z","zplume", "icurve","t");
            CSVDataRow row;
            while (in.read_row(row.istab, row.wind, row.hml,row.mass,row.x, row.sig_x,row.sig_y, row.sig_z, row.zrcp, row.zplume, row.icurve, row.t))
            {
                rows.push_back(row);
            }
        }
        else if(config.computeMode ==0)
        {
            io::CSVReader<6, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
            in.read_header(io::ignore_extra_column, "istab", "speed", "x", "sig_x0", "sig_y0", "sig_z0");
            CSVDataRow row;
            while (in.read_row(row.istab, row.wind, row.x, row.sig_x, row.sig_y, row.sig_z))
            {
                row.y = std::nan("");
                row.z = std::nan("");
                rows.push_back(row);
            }
        }
        return rows;
    }
    static std::vector<CSVDataRow> parseRef2CSV(const std::string &filePath)
    {
        std::vector<CSVDataRow> rows;
        io::CSVReader<6, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
        in.read_header(io::ignore_extra_column, "istab", "speed", "x", "sig_x0", "sig_y0", "sig_z0");
        CSVDataRow row;
        while (in.read_row(row.istab, row.wind, row.x, row.sig_x, row.sig_y, row.sig_z))
        {
            row.y = std::nan("");
            row.z = std::nan("");
            rows.push_back(row);
        }
        return rows;
    }
};
