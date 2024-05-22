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
    thrust::host_vector<float3> _h_loc;
    thrust::host_vector<float3> _h_xVirt;
    thrust::host_vector<float> _h_mass;

    thrust::device_vector<float3> _d_loc;
    thrust::device_vector<float3> _d_xVirt;
    thrust::device_vector<float> _d_mass;


    // Constructor with mass and optional x, xVirt
    Release(thrust::host_vector<float> mass, 
            thrust::host_vector<float3> loc = thrust::host_vector<float3>(1, make_float3(0.f,0.f,0.f)), 
            thrust::host_vector<float3> xVirt = thrust::host_vector<float3>(1, make_float3(0.f,0.f,0.f)))
    {
        _h_mass = mass;
        _h_loc = loc;
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
