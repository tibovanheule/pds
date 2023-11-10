#ifndef CENTRAL_H
#define CENTRAL_H
#include "../../results/results.h"

results Run_Central(int n, int iterations, results results, std::normal_distribution<float> commDelayDist);

#endif