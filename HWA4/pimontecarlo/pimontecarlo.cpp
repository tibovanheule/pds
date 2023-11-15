/***************************************************************************
 *   Copyright (C) 2012-2018 Jan Fostier (jan.fostier@ugent.be)            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cstring>
#include <cmath>
#include <random>
#include <thread>
#include <algorithm>
#include <functional>
#include <mutex>
using namespace std;

mutex pimontecarlo_mutex;

void pimontecarlo(size_t threadID, size_t numThreads, size_t N, double &pi) {
    size_t begin = threadID * N / numThreads;
    size_t end = (threadID + 1) * N / numThreads;

    random_device rd;        // Object to create random seed
    mt19937 generator(rd());    // Mersenne Twister seeded with rd()
    uniform_real_distribution<double> distribution(-1.0, 1.0);

    double pi_local = 0;
    for (size_t i = begin; i < end; i++) {
        double x = distribution(generator);
        double y = distribution(generator);
        if (sqrt(x * x + y * y) < 1.0) pi_local += 4.0 / double(N);
    }
    lock_guard<mutex> lock(pimontecarlo_mutex);
    pi += pi_local;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./pimontecarlo <number of threads>" << endl;
        exit(EXIT_FAILURE);
    }

    size_t numThreads = (size_t) atoi(argv[1]);

    size_t N = 100000000;
    cout << "Generating " << N << " random values using "
         << numThreads << " thread(s)." << endl;

    double pi = 0.0;

    // create threads
    vector<thread> myThreads(numThreads);
    for (size_t i = 0; i < numThreads; i++)
        myThreads[i] = thread(pimontecarlo, i, numThreads, N, ref(pi));

    for_each(myThreads.begin(), myThreads.end(), mem_fn(&thread::join));

    cout.precision(12);
    cout << "Estimated value for pi: " << pi << endl;

    exit(EXIT_SUCCESS);
}
