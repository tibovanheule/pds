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

    if (thisProcess.getNProc() <= 1) exit(-1);

    // procedure initialize
    thisProcess.setPassive(); //participanti  = false;
    // end procedure

    //procedure startElection
    if (thisProcess.getProcessRank() == 0) {
        //SEND FIRST REQUEST, INITIATE ELECTION.
        int send[] = {thisProcess.getProcessRank(), thisProcess.getUid()};

        MPI_Send(send, 2, MPI_INT, 1, 1, MPI_COMM_WORLD);
        thisProcess.setActive();
    }

    /*
    thisProcess.setElectedProcessUid(-1); // electedi = ?
    if (thisProcess.isPassive()) { // if false then Election(i,IDi) is already been sent
        // send message
        int other_process = (thisProcess.getProcessRank() == thisProcess.getNProc() - 1) ? 0 :
                        thisProcess.getProcessRank() + 1;
        int send[] = {thisProcess.getProcessRank(), thisProcess.getUid()};
        MPI_Send(send, 2, MPI_INT, other_process, 1, MPI_COMM_WORLD);
        thisProcess.setActive(); // participation
    }*/
    // end procedure

    // procedure receiveElection(i,ID) . message received at pj
    int recv[] = {0, 0};
    int other_process = (thisProcess.getProcessRank() == 0) ? thisProcess.getNProc() - 1 :
                        thisProcess.getProcessRank() - 1;
    MPI_Recv(recv, 2, MPI_INT, other_process, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    int rank_other_process = recv[0];
    int id_other_process = recv[1];
    if (id_other_process > thisProcess.getUid()){

    }
    
    electedj ?
    forward Election(i, ID)
    participantj
    true
    end
    if
        if ID  IDj ^i
    6 = j
    then
    electedj ?
    if participantj == false
    then
    send Election(j, IDj)
    participantj
    true
    end
    if
        end
        if
            if i == j
    then
            participantj
    false
    electedj j
    send Elected(j)
    end
    if
        end procedure
        procedure
        receiveElected(i).message
    received
    at pj
    if i
        6 = j
    then
            participantj
    false
    electedj i
    forward Elected(i)
    end
    if
        end procedure
    // No need to call MPI_Finalize, this is handled in the destructor of 'Process'. No need to explicitly call the destructor
}