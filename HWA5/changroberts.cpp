/**
 * @file changeroberts.cpp
 * @author tibo vanheule
 */

#include <iostream>
#include "mpi.h"
#include "process.hpp"
#include "changroberts.hpp"

void ChangRoberts::run(int argc, char **argv) const {
    Process thisProcess(argc, argv);

    if (thisProcess.getNProc() <= 1) {
        std::cout << "Only one process: process with rank 0 is the leader." << std::endl;
        return;
    }

    // procedure initialize
    thisProcess.setPassive(); //participanti  = false;
    // end procedure

    //procedure startElection; only for node 0
    if (thisProcess.getProcessRank() == 0) {
#ifdef DEBUG
        std::cout << "Process 0 sending first request" << std::endl;
#endif
        thisProcess.setActive();

        int send[] = {0, thisProcess.getUid(), 1};
        MPI_Send(send, 3, MPI_INT, 1, 1, MPI_COMM_WORLD);

        thisProcess.setElectedProcessUid(-1);
    }
    // end procedure

    MPI_Status status_with_tag;
    int recv[3];
    int other_process = (thisProcess.getProcessRank() == 0) ? thisProcess.getNProc() - 1 :
                        thisProcess.getProcessRank() - 1; // id of sender.
    MPI_Recv(recv, 3, MPI_INT, other_process, MPI_ANY_TAG, MPI_COMM_WORLD, &status_with_tag);

    while (status_with_tag.MPI_TAG == 1) {
        // procedure receiveElection(i,ID) . message received at pj
#ifdef DEBUG
        std::cout << "Received election, rank initiater: " << recv[0] << " id other process" << recv[1] << std::endl;
#endif
        int send_process = (thisProcess.getProcessRank() == thisProcess.getNProc() - 1) ? 0 :
                           thisProcess.getProcessRank() + 1;

        if (recv[1] > thisProcess.getUid()) {
#ifdef DEBUG
            std::cout << "id bigger, process" << thisProcess.getProcessRank() << std::endl;
#endif
            thisProcess.setElectedProcessUid(-1);
            int send[] = {recv[0], recv[1], recv[2] + 1};
            MPI_Send(send, 3, MPI_INT, send_process, 1, MPI_COMM_WORLD);
            thisProcess.setActive();
        }
        if ((recv[1] <= thisProcess.getUid()) && (recv[0] != thisProcess.getProcessRank())) {
#ifdef DEBUG
            std::cout << "kleinere id, process " << thisProcess.getProcessRank() << std::endl;
#endif
            thisProcess.setElectedProcessUid(-1);
            if (thisProcess.isPassive()) {
                int send[] = {thisProcess.getProcessRank(), thisProcess.getUid(), recv[2] + 1};
                MPI_Send(send, 3, MPI_INT, send_process, 1, MPI_COMM_WORLD);
                thisProcess.setActive();
            }
        }
        if (recv[0] == thisProcess.getProcessRank()) {
#ifdef DEBUG
            std::cout << "i==j, process " << thisProcess.getProcessRank() << std::endl;
#endif
            thisProcess.setPassive();
            thisProcess.setElectedProcessUid(recv[1]);
            int send[] = {thisProcess.getProcessRank(), thisProcess.getUid(), recv[2] };
            MPI_Send(send, 3, MPI_INT, send_process, 0, MPI_COMM_WORLD);
        }

        MPI_Recv(recv, 3, MPI_INT, other_process, MPI_ANY_TAG, MPI_COMM_WORLD, &status_with_tag);
    }
#ifdef DEBUG
    std::cout << "out of while loop, process" << thisProcess.getProcessRank() << std::endl;
#endif
    // end procedure
    if (status_with_tag.MPI_TAG == 0) {
#ifdef DEBUG
        std::cout << "Received elected node, rank initiater: " << recv[0] << " id other process" << recv[1]
                  << std::endl;
#endif
        int send_process = (thisProcess.getProcessRank() == thisProcess.getNProc() - 1) ? 0 :
                           thisProcess.getProcessRank() + 1;

        if (recv[0] != thisProcess.getProcessRank()) {
#ifdef DEBUG
            std::cout << "Sending to" << send_process << " from " << thisProcess.getProcessRank()
                      << std::endl;
#endif
            thisProcess.setPassive();
            thisProcess.setElectedProcessUid(recv[1]);
            //recv[2] += 1;
            MPI_Send(recv, 3, MPI_INT, send_process, 0, MPI_COMM_WORLD);
        } else {
            std::cout << "I, process with rank " << thisProcess.getProcessRank() << " am the leader with UID: "
                      << thisProcess.getUid()
                      << "." << std::endl << "There were " << recv[2] << " messages sent to elect a leader."
                      << std::endl;
        }
        // end procedure
    } else {
        exit(0);
    }


    // No need to call MPI_Finalize, this is handled in the destructor of 'Process'. No need to explicitly call the destructor
}