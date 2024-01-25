#ifndef MTPLUME_CONSTANTWINDMET_HPP
#define MTPLUME_CONSTANTWINDMET_HPP
#include <vector>
#include <cuda.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

class ConstantWindMet
{
    public:
    thrust::host_vector<float> _h_hml;
    thrust::host_vector<float> _h_speed;
    thrust::host_vector<int> _h_stability;

    thrust::device_vector<float> _d_hml;
    thrust::device_vector<float> _d_speed;
    thrust::device_vector<int> _d_stability;

/*
    // Constructor
    ConstantWindMet(thrust::host_vector<float> hml, thrust::host_vector<int> stability, thrust::host_vector<float> speed)
    {
        _h_hml = hml;
        _h_stability = stability;
        _h_speed = speed;
    }*/

    // Constructor using member initializer list and move semantics
    ConstantWindMet(thrust::host_vector<float>&& hml, 
                    thrust::host_vector<int>&& stability, 
                    thrust::host_vector<float>&& speed)
        : _h_hml(std::move(hml)), 
          _h_stability(std::move(stability)), 
          _h_speed(std::move(speed))
    {
    }

    //Destructor
    ~ConstantWindMet()
    {
        // Perform any necessary cleanup here
    }

    //function to return size of _h_staiblity;
    int getStabilitySize()
    {
        return _h_stability.size();
    }
};
#endif // MTPLUME_CONSTANTWINDMET_HPP