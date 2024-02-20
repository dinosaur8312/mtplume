
#include "SigmaGrowthPasquillGifford.hpp"
#include "JsonInputHandler.hpp"
#include "CSVParser.hpp"
#include "SigmaInterp.hpp"
// main function
int main(int argc, char *argv[])
{
    std::string configFilePath = ""; // Default to empty, to use input_default.json

    // If a file path is provided as a command-line argument, use it
    if (argc > 1)
    {
        configFilePath = argv[1];
    }

    try
    {
        // Process the JSON file to get the configuration
        SimConfig config = JsonInputHandler::processJsonFile(configFilePath);

        // Further processing with config data...
        //auto coefs = CSVParser::parseCoefCSV(config.coefCSVPath);

        //auto refData = CSVParser::parseRefCSV(config.refCSVPath); 

        SigmaInterp::compareCSVdata(config);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error processing JSON input file: " << e.what() << std::endl;
        return 1;
    }

    if (0)
    {
        thrust::host_vector<float> hml = {500.f};
        // thrust::host_vector<float> windSpeed = {1.f};
        thrust::host_vector<float> windSpeed = {0.8f, 2.3f, 4.1f, 6.7f, 9.6f, 12.f};
        thrust::host_vector<int> stabilityClass = {0, 1, 2, 3, 4, 5};

        printf("pointer address of stabilityClass: %p\n", stabilityClass.data());

        // Create a new instance of the ConstantWindMet class
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

        printf("before real calculation\n");
        fflush(0);

        // Create a new instance of the Plume class
        //   Plume plume;

        // Create a new instance of the PlumeModel class
        //    PlumeModel plumeModel;

        // Create a new instance of the SigmaGrowthPasquillGifford class
        // SigmaGrowthPasquillGifford *sigmaGrowthPasquillGifford = new SigmaGrowthPasquillGifford(sensor, release, met);
        SigmaGrowthPasquillGifford *sigmaGrowthPasquillGifford = new SigmaGrowthPasquillGifford(sensor, release, std::move(met));

        delete release;

        delete sensor;
    }

    return 0;
}