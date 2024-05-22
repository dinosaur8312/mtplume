#ifndef MTPLUME_SENSOR_H
#define MTPLUME_SENSOR_H

#include <vector>
#include <cuda.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include "Util.h"
class Sensor
{
public:
    // Private member variables
    thrust::host_vector<float> _h_x;
    thrust::host_vector<float> _h_z;
    thrust::host_vector<float> _h_t;
    thrust::host_vector<float> _h_level;

    thrust::device_vector<float> _d_x;
    thrust::device_vector<float> _d_z;
    thrust::device_vector<float> _d_t;
    thrust::device_vector<float> _d_level;


    // Constructor
    Sensor(const thrust::host_vector<float>& time, 
           const thrust::host_vector<float>& level,
           const thrust::host_vector<float>& x = thrust::host_vector<float>(), 
           const thrust::host_vector<float>& z = thrust::host_vector<float>()) 
           : _h_t(time), _h_level(level) 
    {
        if (x.empty()) {
            // Generate _h_x using logspace if x is not provided
            // Assuming logspace(start, end, num_elements)
            _h_x = logspace(0.f, 5.f, 91); // Replace with your specific start, end values
        } else {
            _h_x = x;
        }

        if (z.empty()) {
            // Fill _h_z with 0's if z is not provided
            _h_z = thrust::host_vector<float>(_h_x.size(), 0.0f);
        } else {
            _h_z = z;
        }

        // Print the values of _h_x
        std::cout << "Values of _h_x:" << std::endl;
        for (size_t i = 0; i < _h_x.size(); ++i) {
            std::cout << _h_x[i] << " ";
        }
        std::cout << std::endl;
    }



    // Destructor
    ~Sensor()
    {
        // Perform any necessary cleanup here
    }
    // get sensor _h_x size
    int getSensorXSize()
    {
        return _h_x.size();
    }


    // ...
};

#endif // MTPLUME_SENSOR_H