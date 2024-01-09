#include <vector>
#include <cuda.h>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

class ConstantWindMet
{
private:
    thrust::host_vector<float> _h_hml;
    thrust::host_vector<float> _h_speed;
    thrust::host_vector<float> _h_stability;

    thrust::device_vector<float> _d_hml;
    thrust::device_vector<float> _d_speed;
    thrust::device_vector<float> _d_stability;

    public:
    // Constructor
    ConstantWindMet()
    {
    }

    //Destructor
    ~ConstantWindMet()
    {
        // Perform any necessary cleanup here
    }
};
