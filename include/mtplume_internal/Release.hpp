#include <vector>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

class Release
{
private:
    // Private member variables
    thrust::host_vector<float> _h_x;
    thrust::host_vector<float> _h_xVirt;
    thrust::host_vector<float> _h_mass;

    thrust::device_vector<float> _d_x;
    thrust::device_vector<float> _d_xVirt;
    thrust::device_vector<float> _d_mass;


public:
    // Constructor
    Release()
    {
    }

    //destructor
    ~Release()
    {
        // Perform any necessary cleanup here
    }


    // ...
};
