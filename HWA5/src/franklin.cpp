/**
 * @file franklin.cpp
 * @author your name (write your name here)
 */

#include <iostream>
#include "mpi.h"
#include "process.hpp"
#include "franklin.hpp"

void Franklin::run(int argc, char **argv) const
{
    Process thisProcess(argc, argv);

    // Implement your solution here

    // No need to call MPI_Finalize, this is handled in the destructor of 'Process'. No need to explicitly call the destructor
}