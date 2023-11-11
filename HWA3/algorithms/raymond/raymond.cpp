#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

#include <cstdio>
#include <random>
#include "../../csv/csvfile.h"
#include "../../results/results.h"
#include <deque>
// Normal distribution
using std::normal_distribution;

//#define DEBUG

// The node structure for nodes in raymond
// Feel free to add auxiliary functions/structures if desired
struct Node {
    // Define the attributes for the node structure,
    // e.g., id, bool_CS = true if using critical section, ...
    // IMPLEMENT YOUR CODE HERE
    Node *holder; // reference to node that has privilege
    int id; // id of a node
    bool asked = false; // if true, node has sent request to current holder
    bool using_ = false; // true if currently using the critical section

    std::deque<Node *> request_q; // fifo-queue that holds all nodes that requested privilege

    explicit Node(int id) : id(id) {
        this->id = id;
    }

// Define functions for the node structure for the simulation
    // Envisioned Functions below:
    // 1) Assign_Privilege()
    // 2) Initiate_Request()
    // 3) Execute_Critical_Section()
    // 4) Handle_Messages()
    // IMPLEMENT YOUR CODE HERE
    void initiate_request(Node *id_holder, Node &neighbour) {
        // init variables to standard values
        request_q.clear();
        using_ = false;
        asked = false;
        // init variables of neighbour to standard values
        neighbour.request_q.clear();
        neighbour.using_ = false;
        neighbour.asked = false;
        neighbour.holder = id_holder;
    }

    // id_requester node in tree that makes a request to this node.
    void handle_messages(Node *requester, float &cost, std::normal_distribution<float> commDelayDist,
                         std::normal_distribution<float> procDelayDist, std::default_random_engine &generator) {
        request_q.push_back(requester);
#ifdef DEBUG
        std::cout << "REQUEST to Id: " << id << ", requester_node: " << requester->id << std::endl;
#endif
        // algo from paper
        if (!asked && !request_q.empty() && holder->id != id) {
            asked = true;
            // node doesn't hold privilege, return holder so we can request it
            cost += commDelayDist(generator) + procDelayDist(generator);
            holder->handle_messages(this, cost, commDelayDist, procDelayDist, generator);
        } else {
            this->assign_privilege(cost, commDelayDist, generator);
        }
    }

    //assign_to_holder is the value to which the holder sould be set, AKA received privilege.
    void assign_privilege(float &cost, std::normal_distribution<float> commDelayDist,
                          std::default_random_engine &generator) {
        // algo from paper
        if (holder->id == id && !using_ && !request_q.empty()) {
            holder = request_q.front();
            request_q.pop_front();
#ifdef DEBUG
            std::cout << "ASSIGN to Id: " << holder->id << ", assigner_id: " << id << std::endl;
#endif
            asked = false;
            if (holder->id == id) {
                // node itself request privilege
                using_ = true;
                execute_critical_section(cost, commDelayDist, generator);
            } else {
                // an assign has to be made to another node, add comm delay. processing delay is part of
                cost += commDelayDist(generator);
                holder->holder = holder;
                holder->assign_privilege(cost, commDelayDist, generator);
            }

        }
    }

    void execute_critical_section(float &cost, std::normal_distribution<float> commDelayDist,
                                  std::default_random_engine &generator) {
        // do critical work
        using_ = false;
        if (!request_q.empty()) holder->assign_privilege(cost, commDelayDist, generator);
    }
};


// Note: You don’t actually need to include child pointers in the structure but please make sure to start from a binary tree
// since it will affect the average and std deviation results.

// Main Function:
results Run_Raymond(int n, int iterations, results results, std::normal_distribution<float> commDelayDist,
                    std::normal_distribution<float> procDelayDist) {
    printf("Starting Raymond Algorithm with %d nodes. Run it for %d iterations...\n", n, iterations);

    // creating Nodes
    std::vector<Node> nodes;
    // aux variable to keep track of the client delay
    std::vector<float> totalClientDelay(iterations);

    // generator for random seeds in the Gaussian distributions
    std::default_random_engine generator((std::random_device()()));

    // Initialize the raymond-based structure based on the implemented node structure and n
    // IMPLEMENT YOUR CODE HERE
    // construct "edges" so we start with a binary tree
    for (int i = 0; i < n; i++) nodes.emplace_back(i);
    for (Node &node: nodes) {
        int i = node.id;
        if ((2 * i) + 1 < n) node.initiate_request(&node, nodes.at((2 * i) + 1));
        if ((2 * i) + 2 < n) node.initiate_request(&node, nodes.at((2 * i) + 2));
    }
    // initialize the root node as privileged node.
    nodes.at(0).holder = &nodes.at(0);
    // Run for at least 75 different requests and calculate the client delay
    // Feel free to change the code/functions already written if desired or create additional functions if needed!
    // IMPLEMENT YOUR CODE HERE
    std::mt19937 gen(1);
    std::uniform_int_distribution<> intDistribution(0, n - 1);
    for (int iteration = 0; iteration < iterations; iteration++) {
#ifdef DEBUG
        std::cout << "iteration: " << iteration << std::endl;
#endif

        int randomnode = intDistribution(generator);
#ifdef DEBUG
        std::cout << "Chosen random node: " << randomnode << std::endl;
#endif
        nodes.at(randomnode).handle_messages(&nodes.at(randomnode), totalClientDelay.at(iteration), commDelayDist,
                                             procDelayDist, generator);

    }

    // write out to csv
    auto *csv = new csvfile("raymond.csv");
    *csv << "client delay";
    csv->endrow();
    for (auto i: totalClientDelay) {
        *csv << i;
        csv->endrow();
    }
    // calculate results
    Fill_Results(&results, totalClientDelay);
    // return the updated results
    return results;
}

#pragma clang diagnostic pop