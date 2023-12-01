/***************************************************************************
 *   Copyright (C) 2012-2016 Jan Fostier (jan.fostier@ugent.be)            *
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
#include <chrono>
#include <cstring>
#include <cmath>
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>


using namespace std;
using namespace std::chrono;


// ==========================================================================
// TIMING ROUTINES
// ==========================================================================

time_point<std::chrono::system_clock> startTime;

void startChrono() {
    startTime = system_clock::now();
}

double stopChrono() {
    duration<double> elapsed = system_clock::now() - startTime;
    return (elapsed.count());
}

// ==========================================================================
// THREAD ROUTINE
// ==========================================================================

void threadsum(int threadID, int numThreads, const vector<double> &v, vector<double> &global) {
    size_t start = threadID * v.size() / numThreads;
    size_t stop = (threadID + 1) * v.size() / numThreads;

    // double localSum = 0.0; // solution 1
    for (size_t i = start; i < stop; i++) global[threadID * 64] += v[i];// localSum += v[i];
    // global[threadID] = localSum; // solution 1
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./threadsum <number of threads>" << endl;
        exit(EXIT_FAILURE);
    }
    // curly braces are intentional, variables go out of scope this way and fixes all memory leaks
    {
        int numThreads = atoi(argv[1]);
        int N = 100000000;

        cout << "Summing " << N << " values using "
             << numThreads << " thread(s)." << endl;

        // create the input vector v and put some values in v
        vector<double> v(N);
        for (int i = 0; i < N; i++)
            v[i] = i;

        // this vector will contain the partial sum for each thread
        vector<double> localSum(numThreads * 64, 0);

        // create threads. Each thread will compute part of the sum and store
        // its result in localSum[threadID] (threadID = 0, 1, ... numThread-1)
        startChrono();
        vector<thread> myThreads(numThreads);
        for (int i = 0; i < numThreads; i++)
            myThreads[i] = thread(threadsum, i, numThreads, cref(v), ref(localSum));

        for_each(myThreads.begin(), myThreads.end(), mem_fn(&thread::join));

        // calculate global sum
        double globalSum = 0.0;
        for (int i = 0; i < numThreads*64; i++)
            globalSum += localSum[i];

        cout.precision(12);
        cout << "Sum = " << globalSum << endl;
        cout << "Runtime: " << stopChrono() << endl;
    }
    exit(EXIT_SUCCESS);
}
