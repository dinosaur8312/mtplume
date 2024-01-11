#ifndef MTPLUME_RLEASE_H
#define MTPLUME_RLEASE_H
#include <vector>
#include <cuda.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

class Release
{
public:
    // Private member variables
    thrust::host_vector<float> _h_x;
    thrust::host_vector<float> _h_xVirt;
    thrust::host_vector<float> _h_mass;

    thrust::device_vector<float> _d_x;
    thrust::device_vector<float> _d_xVirt;
    thrust::device_vector<float> _d_mass;


    // Constructor with mass and optional x, xVirt
    Release(thrust::host_vector<float> mass, thrust::host_vector<float> x = thrust::host_vector<float>(1, 0.0f), thrust::host_vector<float> xVirt = thrust::host_vector<float>(1, 0.0f))
    {
        _h_mass = mass;
        _h_x = x;
        _h_xVirt = xVirt;
    }
    

    //destructor
    ~Release()
    {
        // Perform any necessary cleanup here
    }


    // ...
};
#endif // MTPLUME_RLEASE_H
