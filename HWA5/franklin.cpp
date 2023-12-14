/**
 * @file franklin.cpp
 * @author your name (write your name here)
 */

#include <iostream>
#include "mpi.h"
#include "process.hpp"
#include "franklin.hpp"

#define DEBUG

void Franklin::run(int argc, char **argv) const {
    Process thisProcess(argc, argv);

    if (thisProcess.getNProc() <= 1) {
        std::cout << "Only one process: process with rank 0 is the leader." << std::endl;
        return;
    }

    // procedure initialize
    thisProcess.setActive(); //participanti  = false;
    // end procedure

    int left = (thisProcess.getProcessRank() == 0) ? thisProcess.getNProc() - 1 :
               thisProcess.getProcessRank() - 1; // id of sender.
    int right = (thisProcess.getProcessRank() == thisProcess.getNProc() - 1) ? 0 :
                thisProcess.getProcessRank() + 1;
    int rounds = 0;
    while (true) {

        if (thisProcess.isActive()) { // If an active node
            rounds++;
            int send[1] = {thisProcess.getUid()};
            int recv_right[1], recv_left[1];
            MPI_Send(send, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
            MPI_Recv(recv_right, 1, MPI_INT, left, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            MPI_Send(send, 1, MPI_INT, left, 0,
                     MPI_COMM_WORLD);
            MPI_Recv(recv_left, 1, MPI_INT, right, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);


            if (std::min(recv_left[0], recv_right[0]) > thisProcess.getUid()) {
                thisProcess.setPassive();
            } else if (std::min(recv_right[0], recv_left[0]) == thisProcess.getUid()) {
                std::cout << "I, process with rank " << thisProcess.getProcessRank() << ", am the leader with UID: "
                          << thisProcess.getUid() << ".\n"
                                                     "There were " << rounds << " rounds needed." << std::endl;
                MPI_Send(send, 1, MPI_INT, right, 1, MPI_COMM_WORLD);
                MPI_Recv(recv_right, 1, MPI_INT, left, 1, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);

                MPI_Send(send, 1, MPI_INT, left, 1,
                         MPI_COMM_WORLD);
                MPI_Recv(recv_left, 1, MPI_INT, right, 1, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);

                break;
            }
        } else if (thisProcess.isPassive()) {
            int recv_right[1], recv_left[1];
            MPI_Status status;
            MPI_Recv(recv_left, 1, MPI_INT, right, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Send(recv_left, 1, MPI_INT, left, status.MPI_TAG, MPI_COMM_WORLD);
            
            MPI_Recv(recv_right, 1, MPI_INT, left, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Send(recv_right, 1, MPI_INT, right, status.MPI_TAG, MPI_COMM_WORLD);


            if (status.MPI_TAG == 1) break;
        }
    }
    // No need to call MPI_Finalize, this is handled in the destructor of 'Process'. No need to explicitly call the destructor
}