# Parallel and Distributed Software Systems
## Homework Assignment 2

### General information 
All required files for this assignment are located in the [HWA2 folder](https://github.ugent.be/jfostier/PDS/tree/master/HWA2) on GitHub. You already cloned this repository to some directory on a `gligarXX` interface node to complete the first homework assignment. From within that directory, run:

```bash
git pull
```

to update your local copy to match the latest version on the GitHub server.

In case you already removed the local copy of the repository, you may want to clone the repository again using:

```bash
git clone https://github.ugent.be/jfostier/PDS.git
```

Note that the repository contains the files for all three homework assignments. At this point, you should only use the files under the `HWA2` folder.

The source should be compiled and run on a `gligarXX` interface node.  In order to compile the source file, use the compile script (relies on CMake internally to generate a Makefile):

```bash
./compile.sh
```

OR you can do it manually using `mpicxx` (wrapper around the `g++` compiler)

```bash
module load OpenMPI
mpicxx -O3 allreduce.cpp -o allreduce
```

To run the example on the interface node:

```bash
module load OpenMPI		# in case not already loaded
mpirun -np 4 ./allreduce
```

where the parameter specified by the `–np` flag denotes the number of parallel processes that should be started. Note that all processes are started on the interface node itself. For this assignment, this is sufficient. In order to start processes on the worker nodes, you will have to write a job script. I refer to the manual on [http://hpc.ugent.be](http://hpc.ugent.be) for information about how to do this (not required for this assignment).

### Modalities
This assignment should be completed **individually**. While it is OK to discuss the solution with fellow students, it is strictly prohibited to exchange and/or copy source code. To submit your solution: 

---
1. Go to [Ufora](http://ufora.ugent.be) and go to the course "Parallel and Distributed Software Systems".
2. Select "Ufora-tools", "Assignments" and "Homework Assignment 2".
3. Click "Add a file" and select your `.zip` file on your PC.
4. Click "Submit" or "Overwrite" at the bottom of the page. You should get a confirmation email when your submission has been successfully received.

Note that you can submit multiple times but that only the last submission is kept and graded. You should hand in a single `<name> <first name>.zip` file which contains a single file:

1. The modified `allreduce.cpp` source file

Make sure your `.zip` file does not contain any paths (directories). The deadline is indicated on the Ufora platform. It is firm and non-negotiable.

---

You should **not** write a report for this assignment.

### 1. Allreduce implementation
Study the C++ source file `allreduce.cpp`.  Implement the allreduce algorithm in the `allreduceRSAG` routine using only MPI point to point communication routines (`MPI_Send`, `MPI_Recv` or `MPI_Sendrecv`). Additionally, you might want to use `MPI_Comm_rank` and `MPI_Comm_size`. The use of other MPI routines is not allowed.

You must implement the **"reduce-scatter – allgather"** algorithm as covered in the lecture. Please refer to the exercise slides for details about this algorithm. Additional instructions can be found in the source file itself.

Do not change the signature of the `allreduceRSAG` routine. We will use automated tests to assess the correctness of your code. Before submitting, make sure it produces correct results!
