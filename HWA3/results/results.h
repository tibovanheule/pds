//
// Created by Jose Santos on 18/10/2022.
//

#ifndef RESULTS_H
#define RESULTS_H

#include <vector>

// Results Structure
struct results {
    float client_mean;
    float client_std;
};

float Get_mean(std::vector<float> &vector);
float Get_std(std::vector<float>& vector, float mean);
void Calc_mean_and_std(std::vector <float> v, float *mean, float *std);
void Fill_Results(results *results, std::vector <float> totalClientDelay);

#endif
