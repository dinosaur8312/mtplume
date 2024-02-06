   #include "SigmaInterp.hpp"
   
   float SigmaInterp::getSigmaX(float x, float wind, int istab) {
        // Simplify the problem by assuming a direct mapping from windSpeed to windIndex (you will need a real method for this)
        // Also, assuming istab can be directly used as an index

        // Find the indices in the x and wind arrays that bracket the given x and wind values
        auto itX = std::lower_bound(_data->x[istab][0].begin(), _data->x[istab][0].end(), x); // Assuming x values are consistent across wind indices
        auto itWind = std::lower_bound(_data->wind[istab].begin(), _data->wind[istab].end(), wind, 
            [&](const std::vector<std::vector<double>>& a, double val) {
                return a[0][0] < val; // Assuming wind values are consistent across x indices
            });

        int m = std::distance(_data->wind[istab].begin(), itWind) - 1;
        int n = std::distance(_data->x[istab][0].begin(), itX) - 1;

        // Boundary checks
        m = std::max(m, 0);
        n = std::max(n, 0);
        int m1 = std::min(m + 1, static_cast<int>(_data->wind[istab].size()) - 1);
        int n1 = std::min(n + 1, static_cast<int>(_data->x[istab][0].size()) - 1);

        // Getting the four corner values for interpolation
        float fQ11 = _data->sig_x[istab][m][n];
        float fQ21 = _data->sig_x[istab][m1][n];
        float fQ12 = _data->sig_x[istab][m][n1];
        float fQ22 = _data->sig_x[istab][m1][n1];

        // Coordinates of the rectangle
        float x1 = _data->x[istab][m][n];
        float x2 = _data->x[istab][m1][n];
        float y1 = _data->wind[istab][m][n];
        float y2 = _data->wind[istab][m][n1];

        // Perform bilinear interpolation
        return bilinearInterpolate(x, wind, fQ11, fQ21, fQ12, fQ22, x1, x2, y1, y2);
    }