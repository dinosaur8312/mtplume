
#include "SigmaGrowthPasquillGifford.hpp"

//main function
int main(int argc, char* argv[])
{
    // Create a new instance of the ConstantWindMet class

    thrust::host_vector<float> hml = {500.f};
    thrust::host_vector<float> windSpeed = {1.f};
    thrust::host_vector<int> stabilityClass = {0,1,2,3,4,5};


    // Use std::unique_ptr and std::move
    auto met = std::make_unique<ConstantWindMet>(std::move(hml), 
                                                 std::move(stabilityClass), 
                                                std::move(windSpeed));


    thrust::host_vector<float> mass = {1.f};
    // Create a new instance of the Release class
    Release *release = new Release(mass);



    thrust::host_vector<float> times = {0.0f, 1000.0f, 2000.0f, 3000.0f};
    thrust::host_vector<float> counterLevels = {1.0e-5f, 1.0e0f, 1.0e5f};
    // Create a new instance of the Sensor class
    Sensor *sensor = new Sensor(times, counterLevels);

    // Create a new instance of the Plume class
 //   Plume plume;

    // Create a new instance of the PlumeModel class
//    PlumeModel plumeModel;

    // Create a new instance of the SigmaGrowthPasquillGifford class
   SigmaGrowthPasquillGifford *sigmaGrowthPasquillGifford = new SigmaGrowthPasquillGifford(sensor, release, met);  


    delete release;

    delete sensor;

    return 0;
}