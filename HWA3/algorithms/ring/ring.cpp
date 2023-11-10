//
// Created by Jose Santos on 02/08/2022.
//

// Include other libraries if needed
#include <cstdio>
#include <random>
#include "../../csv/csvfile.h"
#include "../../results/results.h"

// Normal distribution
using std::normal_distribution;

// Node Structure: Feel free to change if desired
struct node {
    int id;             // unique id
    int holds_token;    // 0 - does not hold token, 1 - holds token
    struct node *left;  // node on the left
    struct node *right; // node on the right
};

typedef struct node node;

// Notes:
//
// // Get a sample from the Gaussian distribution
// float sample = commDelayDist(generator);
// printf("%f\n", sample);

// Function for executing one iteration of the Ring algorithm
// include extra arguments in the function if needed
void Execute_Request_Ring(std::normal_distribution<float> commDelayDist, std::normal_distribution<float> procDelayDist,
                          float &totalClientDelay, std::default_random_engine &generator, node &requester) {
    // Implement the function to calculate the client delay in the ring algorithm.
    // Note: use a normal distribution for the communication and processing delay
    // Arguments: totalClientDelay can be used to keep track of the total client delay of one experimental run.
    // IMPLEMENT YOUR CODE HERE
    node * node_ = &requester;
    totalClientDelay = 0;
    while (node_->holds_token==0){
        node_ = node_->left;
        totalClientDelay += procDelayDist(generator) + commDelayDist(generator);
    }
    if (node_->id!=requester.id) {
        totalClientDelay += commDelayDist(generator);
        requester.holds_token=1;
        node_->holds_token=0;
    }
}

// Main Function:
results Run_Ring(int n, int iterations, results results, std::normal_distribution<float> commDelayDist,
                 std::normal_distribution<float> procDelayDist) {
    printf("Starting Ring Algorithm with %d nodes. Run it for %d iterations...\n", n, iterations);

    // aux variable to keep track of the client delay
    std::vector<float> totalClientDelay(iterations);

    std::vector<node> nodes(n);

    // generator for random seeds in the Gaussian distributions
    std::default_random_engine generator((std::random_device()()));

    // Create the ring-based structure based on the given node structure and n
    // IMPLEMENT YOUR CODE HERE
    for (int i = 0; i < n; i++) {
        nodes[i].id = i;
        nodes[i].holds_token = 0;

        if (i==0) nodes[i].left = &nodes.at((n-1));
        else nodes[i].left = &nodes.at((i-1) % n);
        nodes[i].right = &nodes.at((i + 1) % (n));
    }

    nodes[0].holds_token = 1;
    // Run at least 75 times for different requests and record average and standard deviation (std) results for posterior analysis
    // Feel free to change the code/functions already written if desired or create additional functions if needed!
    // IMPLEMENT YOUR CODE HERE
    std::mt19937 gen(1);
    std::uniform_int_distribution<> intDistribution(0, n-1);

    for (int iteration = 0; iteration < iterations; iteration++) {
        float &test = totalClientDelay.at(iteration);
        int random_node = intDistribution(gen);
        Execute_Request_Ring(commDelayDist, procDelayDist, test, generator,nodes.at(random_node));
    }

    // return the updated results
    auto *csv = new csvfile("ring.csv");
    *csv << "client delay";
    csv->endrow();

    for (auto i: totalClientDelay) {
        *csv << i;
        csv->endrow();
    }
    Fill_Results(&results, totalClientDelay);
    return results;
}

