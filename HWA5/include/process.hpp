/**
 * This file is given and should not be changed
 */

#pragma once

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "mpi.h"

class Process
{
private:
    int uid = -1;
    int processRank = -1;
    int nProc = -1;
    int electedUid = -1;
    bool mIsActive = false;

public:
    Process(int argc, char **argv);
    ~Process();

    // Getters
    bool isActive() const;
    bool isPassive() const;
    int getUid() const;
    int getProcessRank() const;
    int getNProc() const;
    int getElectedProcessUid() const;

    // Setters
    void setPassive();
    void setActive();
    void setElectedProcessUid(int electedUid);
};

inline Process::Process(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    std::srand(std::time(nullptr) + 2 * processRank);
    uid = std::rand();

    std::cout << "Process with rank: " << std::setw(2) << processRank << " has UID: " << std::setw(10) << uid << std::endl;
}

inline Process::~Process()
{
    int isFinalized = 0;
    MPI_Finalized(&isFinalized);

    if (!isFinalized)
    {
        MPI_Finalize();
    }
}

inline bool Process::isActive() const
{
    return mIsActive;
}

inline bool Process::isPassive() const
{
    return !mIsActive;
}

inline int Process::getUid() const
{
    return uid;
}

inline int Process::getProcessRank() const
{
    return processRank;
}

inline int Process::getNProc() const
{
    return nProc;
}

inline int Process::getElectedProcessUid() const
{
    return electedUid;
}

inline void Process::setPassive()
{
    mIsActive = false;
}

inline void Process::setActive()
{
    mIsActive = true;
}

inline void Process::setElectedProcessUid(int electedUid)
{
    this->electedUid = electedUid;
}
