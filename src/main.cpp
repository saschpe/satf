/*
    Copyright (c) 2009 Sascha Peilicke <sasch.pe@gmx.de>

    This application is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This application is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this application; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "boost/threadpool.hpp"
#include "mergesort.h"
#include "quicksort.h"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include <boost/thread.hpp>
#include <boost/timer.hpp>

#include <algorithm>    // for sort()
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
bool less_than(T x, T y)
{
    return x < y;
}

void log(const string &type, const string &name, int size, double time)
{
    // TODO: log stuff to file
}

template <typename T>
void measure_merge_sort(const string &type, vector<T> data)
{
    boost::timer timer;
    merge_sort(data.begin(), data.end(), less_than<T>);
    double elapsed = timer.elapsed();
    log(type, "merge_sort", data.size(), elapsed);
}

template <typename T>
void measure_quick_sort(const string &type, vector<T> data)
{
    boost::timer timer;
    quick_sort(data.begin(), data.end(), less_than<T>);
    double elapsed = timer.elapsed();
    log(type, "quick_sort", data.size(), elapsed);
}

template <typename T>
void measure_std_sort(const string &type, vector<T> data)
{
    boost::timer timer;
    std::sort(data.begin(), data.end());
    double elapsed = timer.elapsed();
    log(type, "std_sort", data.size(), elapsed);
}

int main(int argc, char *argv[])
{
    unsigned int min_size = 1, max_size = 1000;

    // Handle command line arguments
    if (argc == 2) {
        try {
            max_size = boost::lexical_cast<unsigned int>(argv[1]);
        } catch (boost::bad_lexical_cast &) {
            cout << "Bad commandline argument(s) provided!" << endl;
        }
    } else if (argc == 3) {
        try {
            min_size = boost::lexical_cast<unsigned int>(argv[1]);
            max_size = boost::lexical_cast<unsigned int>(argv[2]);
        } catch (boost::bad_lexical_cast &) {
            cout << "Bad commandline argument(s) provided!" << endl;
        }
    } else {
        cout << "Hint: You can provide a custom test data size range:\n\n"
                  << "    " << argv[0] << "[[MIN_SIZE] MAX_SIZE]" << endl;
    }
    cout << "Using" << boost::thread::hardware_concurrency()
              << "threads with a test data size range from" << min_size
              << "to" << max_size << endl;

    // Build a random number generator
    boost::mt19937 rng;
    boost::uniform_int<> dist(min_size, max_size);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > dice(rng, dist);

    // Create our threadpool
    boost::threadpool::pool tp(boost::thread::hardware_concurrency());

    cout << "Measure performance for random data..." << endl;
    vector<int> data;
    for (unsigned int i = min_size; i <= max_size; i++) {
        data.push_back(dice());
        tp.schedule(boost::bind(measure_merge_sort<int>, "random", data));
        tp.schedule(boost::bind(measure_quick_sort<int>, "random", data));
        tp.schedule(boost::bind(measure_std_sort<int>, "random",  data));
    }

    cout << "Measure performance for already sorted data..." << endl;
    data.clear();
    for (unsigned int i = min_size; i <= max_size; i++) {
        data.push_back(i);
        tp.schedule(boost::bind(measure_merge_sort<int>, "sorted", data));
        tp.schedule(boost::bind(measure_quick_sort<int>, "random", data));
        tp.schedule(boost::bind(measure_std_sort<int>, "sorted", data));
    }
    return 0;
}
