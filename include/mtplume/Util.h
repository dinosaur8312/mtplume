#ifndef MTPLUME_UTIL_H
#define MTPLUME_UTIL_H
#include <cmath>
#include <thrust/host_vector.h>

// Helper function to create a logspace vector
thrust::host_vector<float> logspace(float start, float end, size_t num_elements);

#endif // MTPLUME_UTIL_H