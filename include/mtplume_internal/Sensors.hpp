#include <vector>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

class Sensor
{
private:
    // Private member variables
    thrust::host_vector<float> _h_x;
    thrust::host_vector<float> _h_z;
    thrust::host_vector<float> _h_t;
    thrust::host_vector<float> _h_level;

    thrust::device_vector<float> _d_x;
    thrust::device_vector<float> _d_z;
    thrust::device_vector<float> _d_t;
    thrust::device_vector<float> _d_level;


public:
    // Constructor
    Sensor()
    {
    }

    // Destructor
    ~Sensor()
    {
        // Perform any necessary cleanup here
    }
    // ...


    // ...
};
