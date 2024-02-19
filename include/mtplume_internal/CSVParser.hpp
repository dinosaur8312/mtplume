#pragma once

#include "csv.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct CSVDataRow
{
    // int id;
    // int index;
    double x; // Assuming "X" represents a numeric value
    // double D0;
    double sig_x;
    double sig_y;
    double sig_z;
    // double D0U_Q;
    // double coef_y;
    // double expon_y;
    // double coef_z;
    // double expon_z;
    //int iwind; // Iwind as integer type
    int istab; // Istab as integer type
    double wind;
    // char stab; // Stab as character type
    // std::string how; // How as string type
};

struct DataRow
{
    // int id;
    int istab; // Istab as integer type
    double wind;
    // char stab; // Stab as character type
    double x; // Assuming "X" represents a numeric value
    double sig_x;
    double sig_y;
    double sig_z;
    // double xv_x;
    // double xv_y;
    // double xv_z;
    // double diff_x;
    // double diff_y;
    // double diff_z;
    // std::string pass; // How as string type
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
        while (in.read_row(row.x, row.sig_x, row.sig_y, row.sig_z,  row.istab, row.wind))
        {
            rows.push_back(row);
        }
        return rows;
    }
    static std::vector<CSVDataRow> parseRefCSV(const std::string &filePath)
    {
        std::vector<CSVDataRow> rows;
        io::CSVReader<6, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(filePath);
        in.read_header(io::ignore_extra_column, "istab","speed","x", "sig_x0", "sig_y0", "sig_z0"  );
        CSVDataRow row;
        while (in.read_row(row.istab, row.wind, row.x, row.sig_x, row.sig_y, row.sig_z ))
        {
            rows.push_back(row);
        }
        return rows;
    }
};