# Parallel and Distributed Software Systems
## Homework Assignment 5

### General information
All source files required for this assignment are located in the [HWA5 folder](https://github.ugent.be/fdeturck/PDS/tree/main/HWA5) on GitHub. All examples should be compiled and executed using the UGent HPC infrastructure.

A VPN connection to UGent is required when connecting from outside the UGent network. Once connected to the HPC infrastructure, you will be logged on to one of the interface nodes (`gligarXX`). These nodes can be used to compile software and submit jobs to the different clusters (default = Victini), but running software on these interface nodes is generally considered bad practice. For this assignment however, it is fine to run lightweight examples on the interface node. Hence, you may simply execute the commands in the assignments on the interface nodes.

You already cloned the GitHub repository to some directory on an interface node (`gligarXX`) to complete the previous homework assignments. From within that directory, run:

```bash
git pull
```

to update your local copy to match the latest version on the GitHub server.

In case you already removed the local copy of the repository, you may want to clone the repository again using:

```bash
git clone https://github.ugent.be/fdeturck/PDS.git
```

Note that the repository also contains the files for other homework assignments. At this point, you should only use the files under the HWA5 folder.


Your solution should be compiled and run on a `gligarXX` interface node. In order to compile your solution, use the provided compile script (relies on CMake internally to generate a Makefile):

```bash
./compile.sh
```

To run your solution on the interface node:

```bash
module load intel/2023a
mpirun -np 16 ./hwa5 COMMAND
```

Where `COMMAND` should be replaced by either `changroberts` or `franklin` to run the Chang-Roberts or the Franklin Leader Election algorithm respectively.

You can also change the number of parallel processes MPI should start, which is specified by `-np`. Note that all processes are started on the interface node itself. For this assignment, this is sufficient. In order to start processes on the worker nodes, you will have to write a job script. I refer to the manual on [https://docs.hpc.ugent.be](https://docs.hpc.ugent.be/Linux/) for information about how to do this (not required for this assignment).

---
### Submitting your solution
This assignment should be completed **individually**. While it is OK to discuss the solution with fellow students, it is strictly prohibited to exchange and/or copy source code. If you reuse or get inspired by code found online, correctly cite your sources. To submit your solution:

1. Go to [Ufora](http://ufora.ugent.be) and go to the course "Parallel and Distributed Software Systems".
2. Select "Ufora-tools", "Assignments" and "Homework Assignment 5".
3. Click "Add a file" and select your `.zip` file on your PC.
4. Click "Submit" or "Overwrite" at the bottom of the page. You should get a confirmation email when your submission has been successfully received.

Note that you can submit multiple times but that only the last submission is kept and graded. You should hand in a single `<name> <first name>.zip` file with exactly 3 files:

1. The modified `changroberts.cpp` file
2. The modified `franklin.cpp` file
3. A two-page (maximum) pdf report named `report.pdf`

Make sure your `.zip` file does not contain any paths (directories).

---

Your report should contain:

1. Your name
2. The answers to the questions using the following format:
    * **Q1.1**: answer to this question
    * **Q1.2**: answer to this question
    * ...
    * **Q2.1**: answer to this question
    * ...

The maximum size for the report is two A4 pages, 11pt: "In der Beschränkung zeigt sich erst der Meister". There is no need to spend time on making a report with a fancy layout. There is no need to make graphs for this assignment. Though, make sure your answers are clear and unambiguous. The deadline is indicated on the Ufora platform. It is firm and non-negotiable. Note that the Ufora platform will no longer accept solutions submitted after this deadline.

--- 

### 1. Introduction and Chang-Roberts implementation (50%)

Study the C++ header file `process.hpp` located under the `HWA5/include` directory. Take a look at the definition of the constructor of the `Process` class. MPI will be initialized when creating an instance of `Process` and MPI will be safely finalized in the destructor (no need to call the destructor yourself, the destructor gets called automatically when the instance goes out of scope).

In the constructor you will find the following code to generate a UID for the process:

```cpp
std::srand(std::time(nullptr) + 2 * processRank);
uid = std::rand();
```

* **(Q1.1): Concisely describe why the seed for the pseudo-random integer generator depends on the value of `processRank`. What could potentially happen if the seed was not dependent on `processRank`?** (2 points)

Implement the `ChangRoberts::run` method (located in `HWA5/src/changroberts.cpp`), utilizing the Chang-Roberts algorithm. Make sure to keep track of the amount of messages that were sent by the algorithm. The object `thisProcess` is an instance of `Process` and contains all the data that belongs to the current process. Next to basic C++ syntax, you may only use `MPI_Send` and `MPI_Recv` functions.

Assume that **only one** process initiates the election.

Make sure to run the following command to load the correct module, if not loaded already:

```bash
module load intel/2023a
```

To compile and run your solution:

```bash
./compile.sh
mpirun -np 16 ./hwa5 changroberts
```

Expected output can be in the line of:

```bash
[vscXXXXX@gligarXX HWA5]$ mpirun -np 1 ./hwa5 changroberts
Process with rank:  0 has UID: 1681860182
Only one process: process with rank 0 is the leader.

[vscXXXXX@gligarXX HWA5]$ mpirun -np 16 ./hwa5 changroberts
Process with rank:  0 has UID: 1255267680
Process with rank:  2 has UID: 1115121874
Process with rank:  3 has UID: 1584050388
Process with rank:  6 has UID:  834106865
Process with rank:  8 has UID:  691105863
Process with rank: 12 has UID: 1469631662
Process with rank: 14 has UID: 1334089953
Process with rank: 10 has UID:  549255767
Process with rank:  1 has UID: 1724156282
Process with rank:  4 has UID: 2045351173
Process with rank:  5 has UID:  366410438
Process with rank:  7 has UID:  231467126
Process with rank: 11 has UID: 1019111317
Process with rank: 13 has UID: 1947951386
Process with rank: 15 has UID: 1789650213
Process with rank:  9 has UID: 1157561491

I, process with rank 4, am the leader with UID: 2045351173.
There were XX messages sent to elect a leader.
```

* **(Q1.2.1): Run your solution 10 times, with 16 processes. Give the minimum, maximum and average (averaged over 10 runs) amount of messages that your solution needed to elect a leader.** (5 points)
* **(Q1.2.2): Is this in line with the theoretical message complexity given 16 processes are used? Calculate (show your calculations!) the best-case, worst-case and average-case message complexity.** (3 points)

You can measure the execution time of a program using the built in `time` program, as shown below:

```bash
[vscXXXXX@gligarXX HWA5]$ time mpirun -np 16 ./hwa5 changroberts
Process with rank:  3 has UID: 1475699377
Process with rank: 14 has UID: 1219721875
Process with rank:  7 has UID: 1179609785
Process with rank: 10 has UID:  432867043
Process with rank:  0 has UID:   65635916
Process with rank:  4 has UID:  846702521
Process with rank:  5 has UID: 1323686947
Process with rank:  6 has UID:  713084230
Process with rank: 15 has UID: 1689206404
Process with rank:  1 has UID: 1609478765
Process with rank:  2 has UID: 2074337247
Process with rank:  8 has UID: 1648817037
Process with rank: 13 has UID:  753436920
Process with rank:  9 has UID: 2101529487
Process with rank: 11 has UID:  894895876
Process with rank: 12 has UID: 1364345957

I, process with rank 9, am the leader with UID: 2101529487
There were XX messages sent.

real	0mX.XXXs
user	0mX.XXXs
sys  	0mX.XXXs
```

* **(Q1.3.1): Run your solution 10 times using the `time` program to measure the execution time of your solution. Give the minimum, maximum and average (averaged over 10 runs) `real` execution time.** (3 points)
* **(Q1.3.2): Why do we want to know the `real` time and not the time spent in the `user`- or `sys`-mode?** (2 points)
* **Hand in the modified `changroberts.cpp` file in your zip solution (see "Submitting your solution")**


### 2. Franklin implementation (50%)

> ⚠️ It is not allowed to deviate from the original algorithm proposed by Franklin. Other "more efficient" alternatives cannot be considered Franklin's algorithm anymore.

* **(Q2.1.1): Concisely explain the difference between the Franklin algorithm and the Chang-Roberts algorithm** (1 point)
* **(Q2.1.2): Why would anyone choose to use the Franklin algorithm over the Chang-Roberts algorithm or vice versa?** (1 point)

Implement the `Franklin::run` method (located in `HWA5/src/franklin.cpp`), utilizing the Franklin algorithm. Make sure to keep track of the amount of rounds that were needed to elect a leader by the algorithm. The object `thisProcess` is an instance of `Process` and contains all the data that belongs to the current process. Next to basic C++ syntax, you may only use `MPI_Send` and `MPI_Recv` functions.

Make sure to run the following command to load the correct module, if not loaded already:

```bash
module load intel/2023a
```

To compile and run your solution:

```bash
./compile.sh
mpirun -np 16 ./hwa5 franklin
```

Expected output can be in the line of:

```bash
[vscXXXXX@gligarXX HWA5]$ mpirun -np 16 ./hwa5 franklin
Process with rank:  2 has UID:  452462290
Process with rank:  6 has UID:  161895127
Process with rank:  0 has UID:  587902883
Process with rank:  1 has UID: 2140646820
Process with rank:  5 has UID: 1847346050
Process with rank:  7 has UID:  627003225
Process with rank:  9 has UID: 1568169257
Process with rank: 10 has UID:  953950295
Process with rank: 11 has UID:  335876921
Process with rank: 12 has UID:  810848432
Process with rank:  8 has UID:   24536452
Process with rank:  3 has UID:  907524452
Process with rank: 13 has UID: 1276006564
Process with rank: 15 has UID: 1128527501
Process with rank:  4 has UID:  305124725
Process with rank: 14 has UID:  661336375

I, process with rank 1, am the leader with UID: 2140646820.
There were XX rounds needed.
```
* **(Q2.2.1): Run your solution a couple of times, with 16 processes. What is the observed minimum and maximum amount of rounds that your solution needs to elect a leader (including the announcement round)?** (5 points)
* **(Q2.2.2): Calculate (show and explain your calculations!) the theoretical amount of rounds at most needed to elect a leader by 16 processes (including the announcement round). Is this in line with the result from the previous question?** (2 points)
* **(Q2.3.1): Measure the `real` execution `time` of your solution 10 times, with 16 processes. Give the minimum, maximum and average (averaged over 10 runs) `real` execution time.** (2 points)

> ⚠️ It is recommended to run experiments Q1.3.1 and Q2.3.1 in close succession, as external factors such as the variable load on the HPC login node can influence the measurements. If Q1.3.1 was run a while ago, it may be necessary to redo the measurements to ensure a fair comparison.

* **(Q2.3.2): Is there a noticeable execution time difference between Chang-Roberts and Franklin? If so, which one of the two algorithms is more efficient and why? If not, can you explain why these algorithms perform in the same way?** (2 points)
* **(Q2.4): As a passive process, is there a certain order in which messages must be sent or received? If so, explain why that order is important. If not, explain why the order doesn't matter.** (2 points)
* **Hand in the modified `franklin.cpp` file in your zip solution (see "Submitting your solution")**
---
* **(Q3.1): How much time did you spend on this assignment, excluding the time spent studying the lecture notes? (UNGRADED)**
