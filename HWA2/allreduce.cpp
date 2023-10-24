/***************************************************************************
 *   Name Tibo Vanheule
 ***************************************************************************/

/***************************************************************************
 *   Copyright (C) 2012, 2015 Jan Fostier (jan.fostier@ugent.be)           *
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

#include "mpi.h"
#include <cstdlib>
#include <iostream>
#include <random>
#include <cstring>

using namespace std;

/**
 * Wrapper function around MPI_Allreduce (leave this unchanged)
 * @param sendbuf Send buffer containing count doubles (input)
 * @param recvbuf Pre-allocated receive buffer (output)
 * @param count Number of elements in the send and receive buffers
 */
void allreduce(double *sendbuf, double *recvbuf, int count) {
    MPI_Allreduce(sendbuf, recvbuf, count, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
}

/**
 * Wrapper function around MPI_Allreduce (implement reduce-scatter / allgather algorithm)
 * @param sendbuf Send buffer containing count doubles (input)
 * @param recvbuf Pre-allocated receive buffer (output)
 * @param count Number of elements in the send and receive buffers
 */
void allreduceRSAG(double *sendbuf, double *recvbuf, int count) {

    int P, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double *temp = new double[count];
    double *send_recv = new double[count];

    memcpy(temp, sendbuf, sizeof(double) * count);

#ifdef DEBUG
    std::string outputString = "";
    outputString += "Process " + std::to_string(rank);

    outputString += " The SENDBUF is: ";
    for (int i = 0; i < count; ++i) outputString += std::to_string(sendbuf[i]) + " ";
    cout << outputString << endl;
#endif

    // this takes the log of P with basis 2
    double log_p = log(P) / log(2);
    //reduce-scatter
    for (int i = 0; i < log_p; ++i) {
        int n = pow(2, i + 1);
        int count_iter = count / n;
        int other_process = rank;

        if (rank % n < pow(2, i)) {
            other_process += pow(2, i);
            memcpy(send_recv, &temp[count_iter], sizeof(double) * count_iter);
        } else {
            other_process -= pow(2, i);
            memcpy(send_recv, temp, sizeof(double) * count_iter);
        }

#ifdef DEBUG
        std::string outputString = "";
        outputString += "Process " + std::to_string(rank) + " sending to " + std::to_string(other_process);

        outputString += " The SENDING array is: ";
        for (int i = 0; i < count_iter; ++i) outputString += std::to_string(send_recv[i]) + " ";
        cout << outputString << endl;
#endif

        MPI_Send(send_recv, count_iter, MPI_DOUBLE, other_process, 0, MPI_COMM_WORLD);

        MPI_Recv(send_recv, count_iter, MPI_DOUBLE, other_process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


#ifdef DEBUG
        outputString = "";
        outputString += "Process " + std::to_string(rank) + " received";

        outputString += " The received array is: ";
        for (int i = 0; i < count_iter; ++i) outputString += std::to_string(send_recv[i]) + " ";
        cout << outputString << endl;
#endif
        //reduce
        if (rank % n < pow(2, i)) {
            for (int j = 0; j < count_iter; ++j) temp[j] += send_recv[j];

        } else {
            for (int j = 0; j < count_iter; ++j) temp[j] = temp[j + count_iter] + send_recv[j];
        }
    }

#ifdef DEBUG
    outputString = "";
    outputString += "Process " + std::to_string(rank);

    outputString += " The TEMP is: ";
    for (int i = 0; i < count; ++i) outputString += std::to_string(temp[i]) + " ";
    cout << outputString << endl;
#endif

    // allgather:
    for (int i = log_p; i > 0; --i) {

        int other_process = rank;

        int n = pow(2, i);

        other_process += (rank % n < pow(2, i - 1)) ? pow(2, i - 1) : -pow(2, i - 1);

        int count_iter = count / pow(2, i);

        // fill the array that has to be sent
        memcpy(send_recv, temp, sizeof(double) * count_iter);


#ifdef DEBUG
        std::string outputString = "";
        outputString += "ALLGAHER, Process " + std::to_string(rank) + " sending to " + std::to_string(other_process);

        outputString += " The SENDING array is: ";
        for (int i = 0; i < count_iter; ++i) outputString += std::to_string(send_recv[i]) + " ";
        cout << outputString << endl;
#endif

        MPI_Send(send_recv, count_iter, MPI_DOUBLE, other_process, 1, MPI_COMM_WORLD);
        MPI_Recv(send_recv, count_iter, MPI_DOUBLE, other_process, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


#ifdef DEBUG
        outputString = "";
        outputString += "ALLGATHER, Process " + std::to_string(rank) + " received";

        outputString += " The received array is: ";
        for (int i = 0; i < currentCount; ++i) outputString += std::to_string(send_recv[i]) + " ";
        cout << outputString << endl;
#endif

        if (rank % n < pow(2, i - 1)) {
            // received second half
            memcpy(&temp[count_iter], send_recv, sizeof(double) * count_iter);
        } else {
            // received first half
            memcpy(&temp[count_iter], temp, sizeof(double) * count_iter);
            memcpy(temp, send_recv, sizeof(double) * count_iter);
        }
    }

#ifdef DEBUG
    outputString = "";
    outputString += "ALLGATHER, Process " + std::to_string(rank);

    outputString += " The TEMP is: ";
    for (int i = 0; i < count; ++i) outputString += std::to_string(temp[i]) + " ";
    cout << outputString << endl;
#endif

    memcpy(recvbuf, temp, sizeof(double) * count);

    delete[] temp;
    delete[] send_recv;
}

/**
 * Program entry
 */
int main(int argc, char *argv[]) {
    int thisProc, nProc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &thisProc);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    // TEST CODE
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(2, 20);
    int size = distribution(generator) * nProc;
    double *startArray = new double[size];
    for (int i = 0; i < size; ++i) {
        std::uniform_real_distribution<double> distribution_double(0, 2000);
        startArray[i] = distribution_double(generator);
    }

    // The expected value, calculated by allreduce
    double *endArrayExpected = new double[size];
    allreduce(startArray, endArrayExpected, size);

    double *endArrayCalculated = new double[size];
    allreduceRSAG(startArray, endArrayCalculated, size);

    int misses = 0;
    for (int i = 0; i < size; ++i) if (endArrayExpected[i] != endArrayCalculated[i]) misses++;
    if (misses == 0) {
        cout << "No misses!" << endl;
    } else {
        cout << "Number of misses were: " << misses << endl;
#ifdef DEBUG
        std::string outputString += "The expected output of the program is: ";
        for (int i = 0; i < size; ++i) {
            outputString += std::to_string(endArrayExpected[i]) + " ";
        }
        cout << outputString << endl;

#endif
    }

    delete[] startArray;
    delete[] endArrayExpected;
    delete[] endArrayCalculated;

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}
