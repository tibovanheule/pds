#ifndef Raymond_H
#define Raymond_H
#include "../../results/results.h"

results Run_Raymond(int n, int iterations, results results, std::normal_distribution<float> commDelayDist, std::normal_distribution<float> procDelayDist);

#endif