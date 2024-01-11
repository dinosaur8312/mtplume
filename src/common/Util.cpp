// Util.cpp
#include "Util.h"

thrust::host_vector<float> logspace(float start, float end, size_t num_elements)
{
    thrust::host_vector<float> result(num_elements);
    float step = (end - start) / (num_elements - 1);
    for(size_t i = 0; i < num_elements; ++i) {
        result[i] = pow(10.0, start + i * step);
    }
    return result;
}
