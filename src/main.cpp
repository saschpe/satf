/*
    Copyright (c) 2009 Sascha Peilicke <sascha@peilicke.de>

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
#include "countless.h"
#include "heapsort.h"
#include "quicksort.h"
#include "utility.h"

#include <boost/bind.hpp>
#include <boost/date_time.hpp>
#include <boost/program_options.hpp>
#include <boost/random.hpp>
#include <boost/thread.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

using namespace boost;
using namespace std;

template <typename T>
void measure_heap_sort(const vector<T> &data, const string &data_traits = "")
{
    vector<T> tmp = data;   // Work on a copy to not destroy original content
    CountLess<T> less;

    posix_time::ptime start = posix_time::microsec_clock::local_time();
    heap_sort(tmp.begin(), tmp.end(), less);
    posix_time::time_duration td = posix_time::microsec_clock::local_time() - start;
    log("heap_sort", data_traits, tmp.size(), td.total_microseconds(), less.count());
    //print_vector<T>(data, tmp, "heap_sort " + data_traits + ' ');
}

template <typename T>
void measure_quick_sort(const vector<T> &data, const string &data_traits = "")
{
    vector<T> tmp = data;   // Work on a copy to not destroy original content
    CountLess<T> less;

    posix_time::ptime start = posix_time::microsec_clock::local_time();
    quick_sort(tmp.begin(), tmp.end(), less);
    posix_time::time_duration td = posix_time::microsec_clock::local_time() - start;
    log("recursive_quick_sort", data_traits, tmp.size(), td.total_microseconds(), less.count());
    //print_vector<T>(data, tmp, "recursive_quick_sort" + data_traits + ' ');
}

template <typename T>
void measure_std_sort(const vector<T> &data, const string &data_traits = "")
{
    vector<T> tmp = data;   // Work on a copy to not destroy original content
    CountLess<T> less;

    posix_time::ptime start = posix_time::microsec_clock::local_time();
    std::sort(tmp.begin(), tmp.end(), less);
    posix_time::time_duration td = posix_time::microsec_clock::local_time() - start;
    log("std_sort", data_traits, tmp.size(), td.total_microseconds(), less.count());
}

template <typename T>
void measure_std_stable_sort(const vector<T> &data, const string &data_traits = "")
{
    vector<T> tmp = data;   // Work on a copy to not destroy original content
    CountLess<T> less;

    posix_time::ptime start = posix_time::microsec_clock::local_time();
    std::stable_sort(tmp.begin(), tmp.end(), less);
    posix_time::time_duration td = posix_time::microsec_clock::local_time() - start;
    log("std_stable_sort", data_traits, tmp.size(), td.total_microseconds(), less.count());
}

int main(int argc, char *argv[])
{
    unsigned int min_size, max_size;
    unsigned int thread_count;

    // Handle command line arguments
    program_options::options_description desc("Allowed options");
    desc.add_options()
        ("min", program_options::value<unsigned int>(&min_size)->default_value(1), "set minimum data size")
        ("max", program_options::value<unsigned int>(&max_size)->default_value(1000), "set maximum data size")
        ("threads", program_options::value<unsigned int>(&thread_count)->default_value(thread::hardware_concurrency()), "set worker thead count")
        ("help", "display this help and exit");
    program_options::variables_map vm;
    program_options::store(program_options::parse_command_line(argc, argv, desc), vm);
    program_options::notify(vm);

    // Additional command line argument checks
    if (vm.count("help")) {
        cout << "Usage: " << argv[0] << " [OPTIONS]..." << endl << desc << endl;
        return 1;
    }
    if (min_size > max_size) {
        cerr << "error: invalid minimum data size!" << endl;
        return 1;
    }
    cout << "using " << thread_count << " threads and a data size range from " << min_size << " to " << max_size << endl;

    // Create current log directory path
    log_init("logs/");

    // Build a nice random number generator
    mt19937 rng;
    uniform_int<> dist(min_size, max_size);
    variate_generator<mt19937&, uniform_int<> > dice(rng, dist);

    // Create our threadpool
    threadpool::pool tp(thread_count);

    // Create a bunch of data together with printable names
    vector<int> data[4];
    string data_names[4] = {"random_data", "sorted_data", "reverse_sorted_data", "partially_sorted_data"};

    // Iterate over increasing data sizes with each algorithm
    for (unsigned int i = 1; i <= max_size; i++) {
        data[0].push_back(dice());          // Random values
        data[1].push_back(i);               // Sorted values
        data[2].push_back(max_size - i);    // Reverse sorted values
        data[3].push_back(i);               // Partially sorted values
        if (i / (float)max_size == 0.5) {
            // Results in 50% sorted data, not really 'partial' but does the trick
            sort(data[3].begin(), data[3].end());
        }

        // Make sure we first reach the desired data size before doing anything useful
        if (i < min_size) {
            continue;
        }

        // Apply every algorithm to all data types
        for (unsigned int j = 0; j < 4; j++) {
            tp.schedule(bind(measure_heap_sort<int>, data[j], data_names[j]));
            //tp.schedule(bind(measure_quick_sort<int>, data[j], data_names[j]));
            tp.schedule(bind(measure_std_sort<int>, data[j], data_names[j]));
            tp.schedule(bind(measure_std_stable_sort<int>, data[j], data_names[j]));
        }
    }
    return 0;
}
