#include "ConstantWindMet.hpp"
#include "Release.hpp"
#include "Sensors.hpp"

//main function
int main(int argc, char* argv[])
{
    // Create a new instance of the ConstantWindMet class

    thrust::host_vector<float> hml = {500.f};
    thrust::host_vector<float> windSpeed = {1.f};
    thrust::host_vector<int> stabilityClass = {0,1,2,3,4,5};

    ConstantWindMet *met = new ConstantWindMet(hml, stabilityClass, windSpeed);

    // Create a new instance of the Release class
    Release release;

    // Create a new instance of the Sensor class
    Sensor sensor;

    // Create a new instance of the Plume class
 //   Plume plume;

    // Create a new instance of the PlumeModel class
//    PlumeModel plumeModel;

    delete met;


    return 0;
}