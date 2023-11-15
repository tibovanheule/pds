// Your name: Jan-Pieter Baert
/***************************************************************************
 *   Copyright (C) 2012-2016 Jan Fostier (jan.fostier@ugent.be)            *
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

#include <cmath>
#include <condition_variable>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

using namespace std;
vector<int> stack;
mutex donut_mutex;
condition_variable donut_empty;
condition_variable donut_full;

// ==========================================================================
// THREAD ROUTINES
// ==========================================================================

int bake() {
  // sleep between 100 and 1000 ms (randomly chosen)
  mt19937 generator(random_device{}());
  uniform_int_distribution<> dist(100, 1000);
  this_thread::sleep_for(chrono::milliseconds{dist(generator)});

  cout << "Baked donut...\n";
  return 0;
}

void producer() {
  while (true) {
    int element = bake();

    unique_lock<mutex> lk(donut_mutex);
    if (stack.size() == 10) {
      // wait for the consumer to eat a donut
      donut_full.wait(lk, [] { return stack.size() != 10; });
    }

    stack.push_back(element); // push "created" element onto stack

    // unlock before notifying to avoid waking up the waiting thread to then be
    // blocked again
    lk.unlock();
    if (stack.size() == 1) {
      donut_empty.notify_all();
    }
  }
}

void eat(int donut) {
  // sleep between 100 and 1000 ms (randomly chosen)
  mt19937 generator(random_device{}());
  uniform_int_distribution<> dist(100, 1000);
  this_thread::sleep_for(chrono::milliseconds{dist(generator)});

  cout << "Ate donut...\n";
}

void consumer() {
  while (true) {
    unique_lock<mutex> lk(donut_mutex);
    if (stack.empty()) {
      // wait for the consumer to eat a donut
      donut_empty.wait(lk, [] { return stack.size() != 0; });
    }

    int donut = stack.back(); // get element
    stack.pop_back();         // remove element
    eat(donut);

    lk.unlock();
    if (stack.size() == 9) {
      donut_full.notify_all();
    }
    //     wake producer thread
  }
}

int main() {
  cout << "Warning: this code runs forever..." << endl;

  // create threads
  thread t1(producer);
  thread t2(consumer);

  // this point will never be reached ...
  t1.join();
  t2.join();

  exit(EXIT_SUCCESS);
}
