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
#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include <boost/thread.hpp>

#include <algorithm>    // for sort()
#include <iostream>
#include <vector>

using namespace boost;
using namespace std;

static filesystem::path g_log_dir;
static mutex g_log_mutex;

void log(const string &data_traits, const string &name, int size, unsigned int time_msecs)
{
    lock_guard<mutex> lock(g_log_mutex);

    filesystem::path log_dir = filesystem::complete(data_traits, g_log_dir);
    filesystem::path log_file = filesystem::complete(name, log_dir);

    filesystem::create_directory(log_dir);
    filesystem::ofstream ofs(log_file, ios_base::app | ios_base::out);
    ofs << size << ' ' << time_msecs << endl;
}

template <typename T>
void measure_merge_sort(const string &data_traits, vector<T> data)
{
    posix_time::ptime start = posix_time::microsec_clock::universal_time();
    merge_sort(data.begin(), data.end(), std::less<T>());
    posix_time::time_duration td = posix_time::microsec_clock::universal_time() - start;

    log(data_traits, "merge_sort", data.size(), td.total_microseconds());
}

template <typename T>
void measure_quick_sort(const string &data_traits, vector<T> data)
{
    posix_time::ptime start = posix_time::microsec_clock::universal_time();
    quick_sort(data.begin(), data.end(), std::less<T>());
    posix_time::time_duration td = posix_time::microsec_clock::universal_time() - start;

    log(data_traits, "quick_sort", data.size(), td.total_microseconds());
}

template <typename T>
void measure_std_sort(const string &data_traits, vector<T> data)
{
    posix_time::ptime start = posix_time::microsec_clock::universal_time();
    std::sort(data.begin(), data.end());
    posix_time::time_duration td = posix_time::microsec_clock::universal_time() - start;

    log(data_traits, "std_sort", data.size(), td.total_microseconds());
}

int main(int argc, char *argv[])
{
    unsigned int min_size = 1, max_size = 1000;

    // Handle command line arguments
    if (argc == 2) {
        try {
            max_size = lexical_cast<unsigned int>(argv[1]);
        } catch (bad_lexical_cast &) {
            cerr << "Bad commandline argument(s) provided!" << endl;
            return 1;
        }
    } else if (argc == 3) {
        try {
            min_size = lexical_cast<unsigned int>(argv[1]);
            max_size = lexical_cast<unsigned int>(argv[2]);
        } catch (bad_lexical_cast &) {
            cerr << "Bad commandline argument(s) provided!" << endl;
            return 1;
        }
    } else {
        cout << "Hint: You can provide a custom test data size range:\n\n"
             << "    " << argv[0] << " [[MIN_SIZE] MAX_SIZE]\n" << endl;
    }
    cout << "Using " << thread::hardware_concurrency()
         << " threads with a test data size range from " << min_size
         << " to " << max_size << endl;

    // Create current log directory path
    g_log_dir = filesystem::complete("logs/", filesystem::current_path());
    filesystem::create_directory(g_log_dir);
    string now = lexical_cast<string>(boost::posix_time::second_clock::universal_time());
    replace_all(now, " ", "_");
    g_log_dir = filesystem::complete(now, g_log_dir);
    filesystem::create_directory(g_log_dir);
    if (!filesystem::exists(g_log_dir)) {
        cerr << "Unable to create log directory " << g_log_dir << endl;
        return 1;
    } else {
        cout << "Created log directory " << g_log_dir << endl;
    }

    // Build a random number generator
    mt19937 rng;
    uniform_int<> dist(min_size, max_size);
    variate_generator<mt19937&, uniform_int<> > dice(rng, dist);

    // Create our threadpool
    threadpool::pool tp(thread::hardware_concurrency());

    cout << "Measure performance for random data..." << endl;
    vector<int> data(max_size);
    for (unsigned int i = 0; i < max_size; i++) {
        data.push_back(dice());
        if (i < min_size) {
            continue;
        }
        tp.schedule(bind(measure_merge_sort<int>, "random", data));
        tp.schedule(bind(measure_quick_sort<int>, "random", data));
        tp.schedule(bind(measure_std_sort<int>, "random",  data));
    }

    cout << "Measure performance for sorted data..." << endl;
    data.clear();
    for (unsigned int i = 0; i < max_size; i++) {
        data.push_back(i);
        if (i < min_size) {
            continue;
        }
        tp.schedule(bind(measure_merge_sort<int>, "sorted", data));
        tp.schedule(bind(measure_quick_sort<int>, "sorted", data));
        tp.schedule(bind(measure_std_sort<int>, "sorted", data));
    }
    return 0;
}
