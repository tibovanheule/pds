//
// Created by Jose Santos on 09/08/2022.
//

#include <cstdio>
#include <random>
#include "algorithms/central/central.h"
#include "algorithms/raymond/raymond.h"
#include "algorithms/ring/ring.h"
#include <cstring>
#include "results/results.h"

// For gaussian distributions
using std::normal_distribution;

// Main function, usage: <algorithm> <num_iterations>, algorithm = raymond, central or ring;
// Can be changed if needed, to include extra arguments in the three algorithm functions!

int main(int argc, char *argv[]) {
    printf("Program Name: %s \n",argv[0]);
    printf("Test Name: %s \n", argv[1]);
    printf("Num. iterations: %s \n", argv[2]);

    // Initialize results structure
    results results{0.0, 0.0};

    // Number of Iterations
    int iterations = atoi(argv[2]);

    // Assume N equal to 100 (number of nodes)
    int n = 100;

    // Gaussian distributions:
    // communication delay: mean 30 ms, std = 5 ms.
    // processing delay: mean 15 ms, std = 2 ms.
    std::normal_distribution<float> commDelayDist(30.0,5.0);
    std::normal_distribution<float> procDelayDist(15.0,2.0);

    if (strcmp(argv[1], "central") == 0) {
        results = Run_Central(n, iterations, results, commDelayDist);
    } else if (strcmp(argv[1], "ring") == 0) {
        results = Run_Ring(n, iterations, results, commDelayDist, procDelayDist);
    } else if (strcmp(argv[1], "raymond") == 0) {
        results = Run_Raymond(n, iterations, results, commDelayDist, procDelayDist);
    } else {
        printf("Unspecified Test: %s \n", argv[1]);
        return -1;
    }

    // Print results
    printf("\n");
    printf("Avg. Client Delay: %f \n", results.client_mean);
    printf("Std. Client Delay: %f \n", results.client_std);
}