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

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cstring>
#include <cmath>
#include <thread>
#include <vector>
#include <random>

using namespace std;
vector<int> stack;

// ==========================================================================
// THREAD ROUTINES
// ==========================================================================

int bake()
{
        // sleep between 100 and 1000 ms (randomly chosen)
        mt19937 generator(random_device{}());
        uniform_int_distribution<> dist(100, 1000);
        this_thread::sleep_for(chrono::milliseconds{dist(generator)});

        cout << "Baked donut...\n";
        return 0;
}

void producer()
{
        while (true) {
                int element = bake();

                // if (stack.size() == 10)
                //      thread goes to sleep

                stack.push_back(element); // push "created" element onto stack

                // if (stack.size() == 1)
                //      wake consumer thread
        }
}

void eat(int donut)
{
        // sleep between 100 and 1000 ms (randomly chosen)
        mt19937 generator(random_device{}());
        uniform_int_distribution<> dist(100, 1000);
        this_thread::sleep_for(chrono::milliseconds{dist(generator)});

        cout << "Ate donut...\n";
}

void consumer()
{
        while (true) {
                // if (stack.empty())
                //     thread goes to sleep

                int donut = stack.back(); // get element
                stack.pop_back(); // remove element

                // if (stack.size() == 9)
                //     wake producer thread

                eat(donut);
        }
}

int main()
{
        cout << "Warning: this code runs forever..." << endl;

        // create threads
        thread t1(producer);
        thread t2(consumer);

        // this point will never be reached ...
        t1.join();
        t2.join();

        exit(EXIT_SUCCESS);
}
