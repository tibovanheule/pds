#include <cmath>
#include <numeric>
#include <algorithm>
#include <utility>
#include "results.h"

// returns the mean of a vector
float Get_mean(std::vector<float> &vector) {
    float sum = 0;
    for (float& element : vector) {
        sum += element;
    }
    return sum / vector.size();
}

// returns the std of a vector
float Get_std(std::vector<float>& vector, float mean) {
    float sum = 0;
    for (float& element : vector) {
        sum += std::pow(element - mean, 2);
    }
    return std::sqrt(sum / vector.size());
}

// Function to calculate both mean and std: based on https://stackoverflow.com/a/12405793
void Calc_mean_and_std(std::vector <float> v, float *mean, float *std){
    double sum = std::accumulate(std::begin(v), std::end(v), 0.0);
    *mean =  sum / v.size();

    double accum = 0.0;
    std::for_each (std::begin(v), std::end(v), [&](const double d) {
        accum += (d - (*mean)) * (d - (*mean));
    });

    *std = sqrt(accum / (v.size()-1));
}

// Function to fill results structure
void Fill_Results(results *results, std::vector <float> totalClientDelay){
    Calc_mean_and_std(std::move(totalClientDelay), &((*results).client_mean), &((*results).client_std));
}