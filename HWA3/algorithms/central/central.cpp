//
// Created by Jose Santos on 08/08/2022.
//

// Include other libraries if needed
#include <cstdio>
#include <random>
#include <vector>
#include "../../csv/csvfile.h"
#include "../../results/results.h"

// Normal distribution
using std::normal_distribution;

// Central Structure: Modify if needed
struct central {
    float process_delay; // Processing time of the Central Server
};

typedef struct central central;

// Notes:
//
// // Get a sample from the Gaussian distribution
// float sample = commDelayDist(generator);
// printf("%f\n", sample);

// Function for executing one iteration of the central algorithm:
// include extra arguments in the function if needed
// commDelayDist = communication  delay distribution
int Execute_Request_CENTRAL(std::normal_distribution<float> commDelayDist, float &totalClientDelay,
                            std::default_random_engine &generator) {
    // Implement the function to calculate the client delay in the central server.
    // Arguments: totalClientDelay can be used to keep track of the total client delay of one experimental run.
    // IMPLEMENT YOUR CODE HERE
    // request cost time,
    float request_msg = commDelayDist(generator); // request message
    float privilege_msg = commDelayDist(generator); // privilege message = grant
    totalClientDelay = privilege_msg + request_msg;
    return 0;
}

// Main Function:
results Run_Central(int n, int iterations, results results, std::normal_distribution<float> commDelayDist) {
    printf("Starting Central Algorithm with %d nodes. Run it for %d iterations...\n", n, iterations);

    // init central
    central central = {expf(n / 20.0f) * 40.0f};

    std::cout << "Processing delay: " << central.process_delay << std::endl;
    // aux variable to keep track of the client delay
    std::vector<float> totalClientDelay(iterations);

    // generator for random seeds in the Gaussian distributions
    std::default_random_engine generator((std::random_device()()));

    // Define the processing delay of the Central server based on the provided formula and n
    // done in declaration of central variable
    // expf(n/20.0) * 40.0f

    // Run for at least 75 requests and calculate the client delay
    // Feel free to change the code/functions already written if desired or create additional functions if needed!
    // IMPLEMENT YOUR CODE HERE
    for (int iteration = 0; iteration < iterations; iteration++) {
        int _ = Execute_Request_CENTRAL(commDelayDist, totalClientDelay.at(iteration), generator);
        float &test = totalClientDelay.at(iteration);
        test += central.process_delay;
    }

    Fill_Results(&results, totalClientDelay);
    // return the updated results

    auto *csv = new csvfile("central.csv");
    *csv << "client delay";
    csv->endrow();
    for (auto i: totalClientDelay) {
        *csv << i;
        csv->endrow();
    }
    return results;
}