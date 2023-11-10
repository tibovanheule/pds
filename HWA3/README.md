# Parallel and Distributed Software Systems

## Homework Assignment 3 - Distributed Mutual Exclusion

### UGent HPC infrastructure

All source files required for this assignment are located in the [HWA3 folder](https://github.ugent.be/fdeturck/PDS/tree/main/HWA3) on GitHub. 
All examples should be compiled and executed using the UGent HPC infrastructure. 
You will need a VSC account to access these clusters. 
Instructions on how to apply for this account can be found in the 'Announcements' section on [Ufora](http://ufora.ugent.be).

After your account has been approved, you may want to read the [HPC Tutorial](https://www.ugent.be/hpc/en/support/documentation.htm). 
It explains (among many other things) how to connect to the HPC infrastructure and how to copy files between your system and the UGent HPC. 
Note that there are different versions of this tutorial, depending on the operating system you are using.

Once connected to the HPC infrastructure, you will be logged on to one of the interface nodes (`gligarXX`). These nodes can be used to compile software and submit jobs to the different clusters (default = Victini), but running software on these interface nodes is generally considered bad practice. For this assignment however, it is fine to run lightweight examples on the interface node. Hence, you may simply execute the commands in the assignments on the interface nodes.

The project should be compiled and run on a `gligarXX` interface node. In order to compile all source files, please use 
the provided [compile.sh](compile.sh) script (that relies on CMake internally to generate a Makefile):

```bash
./compile.sh
```

Note: The code should only be compiled/executed after you implemented the missing functions. 
You will need to complete a few functions for this exercise. 

### Submitting your solution

This assignment should be completed **individually**. 
While it is OK to discuss the solution with fellow students, it is strictly prohibited to exchange and/or copy source code. 
If you reuse or get inspired by code found online, correctly cite your sources. 
To submit your solution:

---
1. Go to [Ufora](http://ufora.ugent.be) and go to the course "Parallel and Distributed Software Systems".
2. Select "Ufora-tools", "Assignments" and "Homework Assignment 3".
3. Click "Add a file" and select your `.zip` file on your PC.
4. Click "Submit" or "Overwrite" at the bottom of the page. You should get a confirmation email when your submission has been successfully received.

Note that you can submit multiple times but that only the last submission is kept and graded. You should hand in a single `<name> <first name>.zip` file with at least four files:

1. The modified `algorithms/raymond/raymond.cpp` source file and additional files if created.
2. The modified `algorithms/central/central.cpp` source file and additional files if created.
3. The modified `algorithms/ring/ring.cpp` source file and additional files if created.
4. A one-page pdf report named `report.pdf`

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

The maximum size for the report is one A4 page, 11pt: "In der Beschränkung zeigt sich erst der Meister". 
There is no need to spend time on making a report with a fancy layout. 
There is no need to produce graphs for this assignment. 
Though, make sure your answers are clear and unambiguous.
The deadline is indicated on the Ufora platform.
This deadline is firm and non-negotiable. 
Note that the Ufora platform will no longer accept solutions submitted after this deadline.

### 1. Implementation of distributed mutual exclusion algorithms

Download the source files from Github from one of  the `gligarXX` interface nodes by cloning the git repository:

```bash
git clone https://github.ugent.be/fdeturck/PDS.git
```

Note that the repository contains the files for all five homework assignments. 
At this point, you should **only use the files under the `HWA3` folder**.

Locate the C++ source files in the `PDS/HWA3/` folder. 
Read the source code and make sure you understand it.

For this exercise, please simulate the behavior of the three algorithms located in the main folder `algorithms/`. Central, Ring and Raymond.
Find here all skeletons for the three algorithms: [Central](algorithms/central/central.cpp), [Ring](algorithms/ring/ring.cpp) and [Raymond](algorithms/raymond/raymond.cpp).

Consider the following exercise:

Assume that the communication delay always follows a **Gaussian distribution with a mean of 30 milliseconds and standard deviation of 5 milliseconds**.

1. The **central server** algorithm, where the processing delay in the central server equals 40 * e^(N/20) milliseconds (for both request- and leave-messages), where N represents the number of clients involved and e denotes the exponential function. Thus, the processing delay is constant. 

2. The **ring-based** algorithm with N different nodes. The processing delay in each node (to receive and send the token, or receive and keep the token) follows a gaussian distribution with a mean of 15 milliseconds and standard deviation of 2 milliseconds.

3. The **raymond’s** algorithm for distributed mutual exclusion, with N different nodes. The processing delay in each node (to receive and send the token, or receive and keep the token) follows a gaussian distribution with a mean of 15 milliseconds and standard deviation of 2 milliseconds. Each node has a pointer to the parent node (value=NULL for the root node), and each node has a maximum of two child nodes (binary tree).

Assume that **N equals 100**, implement the three algorithms above in the C++ programming language 
and calculate the **client delay** for **75 iterations assuming a different node starts 
the request (i.e., 1 iteration represents 1 request)**. 

Determine the **average** and the **standard deviation** of the **client delay** for the three different algorithms.

#### Important Notes  

The document [raymond.pdf](papers/raymond.pdf) contains the main publication about the raymond's tree-based algorithm.
Feel free to read the document to familiarize yourself with the Raymond algorithm.

The file [csvfile.h](csv/csvfile.h) contains a csv library that can be used to save experimental runs in a .csv file. 
Please feel free to use it in your implementation.

The file [results.h](results/results.cpp) contains several auxiliary functions that can be used to 
calculate the average and the standard deviation of several algorithm iterations. 
Please feel free to use it in your implementation.

All algorithms (Central, Ring and Raymond) should run in a simulation-based environment. 
The goal of HWA3 is to produce and keep track of the totalClientDelay. 
We do not expect you to mimic the full behavior of the algorithms.

The number of iterations represents the total number of executions of the algorithms. 
Consider 75 iterations starting with a different request (i.e., a random node starting the request). 
Consider requests are sent sequentially since we do not expect students to come up with a solution where multiple requests can be sent in parallel.

For instance, in the central algorithm, the client delay will be:

```bash
client_delay = request_msg + processing_time + privilege_msg
```

Please do not forget to load the required modules (compile.sh) in the HPC shell 
before running the script since when running it, the files are built in a new shell. 
So, the modules still need to be loaded:

```bash
module load intel/2021b
module load CMake/3.22.1-GCCcore-11.2.0
```

#### Questions

Please answer the following questions in the report: 

### 1. Performance of the three algorithms (100%)

**Q1.1: Which results (average and standard deviation) did you obtain for the client delay of the three algorithms? (6 points)**

**Q1.2: Which algorithm seems to achieve higher performance? 
Also, are there any significant differences in the performance 
of the central and the ring-based algorithm? Explain the reason why. (6 points)**

**Q1.3: Based on the standard deviation results, which conclusions can you draw from these? (4 points)**

**Q1.4: What are the drawbacks of the Raymond’s tree-based algorithm if any? (4 points)**

### 2. Statistics (ungraded)

**Q2.1 How much time did you spend on this assignment, excluding the time spent studying the lecture notes?**