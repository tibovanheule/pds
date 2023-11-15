# Parallel and Distributed Software Systems
## Homework Assignment 3

### General information
All source files required for this assignment are located in the [HWA3 folder](https://github.ugent.be/jfostier/PDS/tree/master/HWA3) on GitHub. All examples should be compiled and executed using the UGent HPC infrastructure.

A VPN connection to UGent is required when connecting from outside the UGent network. Once connected to the HPC infrastructure, you will be logged on to one of the interface nodes (`gligarXX`). These nodes can be used to compile software and submit jobs to the different clusters (default = Victini), but running software on these interface nodes is generally considered bad practice. For this assignment however, it is fine to run lightweight examples on the interface node. Hence, you may simply execute the commands in the assignments on the interface nodes.

You already cloned the GitHub repository to some directory on an interface node (`gligarXX`) to complete the first and second homework assignments. From within that directory, run:

```bash
git pull
```

to update your local copy to match the latest version on the GitHub server.

In case you already removed the local copy of the repository, you may want to clone the repository again using:

```bash
git clone https://github.ugent.be/jfostier/PDS.git
```

Note that the repository contains the files for all three homework assignments. At this point, you should only use the files under the HWA3 folder.

### Submitting your solution
This assignment should be completed **individually**. While it is OK to discuss the solution with fellow students, it is strictly prohibited to exchange and/or copy source code. To submit your solution:

---
1. Go to [Ufora](http://ufora.ugent.be) and go to the course "Parallel and Distributed Software Systems".
2. Select "Ufora-tools", "Assignments" and "Homework Assignment 3".
3. Click "Add a file" and select your `.zip` file on your PC.
4. Click "Submit" or "Overwrite" at the bottom of the page. You should get a confirmation email when your submission has been successfully received.

Note that you can submit multiple times but that only the last submission is kept and graded. You should hand in a single `<name> <first name>.zip` file with exactly four files:

1. The modified `pimontecarlo.cpp` file
2. The modified `threadsum.cpp` file
3. The modified `prodcons.cpp` file
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

The maximum size for the report is one A4 page, 11pt: "In der Beschränkung zeigt sich erst der Meister". There is no need to spend time on making a report with a fancy layour. There is no need to make graphs for this assignment. Though, make sure your answers are clear and unambiguous. **The deadline is Tuesday, December 7th 10:00 am, CET**. This deadline is firm and non-negotiable. Note that the Ufora platform will no longer accept solutions submitted after this deadline.


### 1. Correctness of multithreaded code
Study the C++ source file `pimontecarlo.cpp` under the `HWA3/pimontecarlo` directory. Study the source code. Compile and run the code using the GCC compiler:

```bash
module load Valgrind	# This also loads GCC
g++ -g3 pimontecarlo.cpp -o pimontecarlo -lpthread
./pimontecarlo <number of threads>
```

Note the inclusion of debugging information (`-g3` flag). The program can likely be executed without crashing, but is incorrect.  Certain race conditions can be detected using the Valgrind software, e.g.:

```bash
valgrind --tool=helgrind ./pimontecarlo 2
```

By studying both the output of Valgrind and/or the program code itself, you should be able to correct this program. Hint: there are 2 distinct data races in the code.

* **(Q1.1): pinpoint the first error + explain why the code is incorrect**
* **(Q1.2): pinpoint the second error + explain why the code is incorrect**
* **Correct the source code and hand in the modified source.**

### 2. Influence of memory access patterns on software performance
Study the C++ source file `threadsum.cpp` under the `HWA3/threadsum` directory.  Compile and run the code using the GCC compiler as follows:

```bash
g++ -O0 threadsum.cpp -o threadsum -lpthread
./threadsum <number of threads>
```

Note the absence of compiler optimization (`-O0`). This code sums the elements in an array in a multi-threaded fashion, without the use of a mutex or atomic variables.  The code is functionally correct.

Run the code on a `gligarXX` interface node  using 1, 2, 4, 8 and 16 threads.

* **(Q2.1) Report the runtimes and speedup.**

Now recompile the source using full optimizations:

```bash
  g++ -O3 threadsum.cpp -o threadsum -lpthread
```

Again run the code on a `gligarXX` interface node using 1, 2, 4, 8 and 16 threads.

* **(Q2.2) Report the runtimes and speedup.**
* **(Q2.3) Explain different behavior in obtained speedup.**
* **Modify the source code such that any speedup problems are resolved, regardless of the optimization chosen.  Hand in the modified source.**

### 3. Synchronization using condition variables
Study the C++ source file `prodcons.cpp` under the `HWA3/prodcons` directory.  This code contains skeleton code for the producer-consumer example that we've seen during the lectures.  No synchronization is present in the code.

* **Implement the producer/consumer pattern using condition variables. Hand in the modified code.**
