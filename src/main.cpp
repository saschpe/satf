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
#include "bubbleextsort.h"
#include "heapsort.h"
#include "quicksort.h"
#include "splitsort.h"

#include <boost/bind.hpp>
#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/random.hpp>
#include <boost/thread.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

using namespace boost;
using namespace std;

static filesystem::path g_log_dir;
static bool g_verbose = false;

void log(const string &data_traits, const string &name, int size, unsigned int time_msecs)
{
    static mutex log_mutex;

    lock_guard<mutex> lock(log_mutex);
    filesystem::path log_dir = filesystem::complete(data_traits, g_log_dir);
    filesystem::path log_file = filesystem::complete(name, log_dir);
    filesystem::create_directory(log_dir);
    filesystem::ofstream ofs(log_file, ios_base::app | ios_base::out);
    ofs << size << ' ' << time_msecs << endl;
}

template <typename T>
void print(const vector<T> &before, const vector<T> &after, const string &line_prefix = "")
{
    static mutex print_mutex;

    lock_guard<mutex> lock(print_mutex);
    if (g_verbose) {
        cout << line_prefix << "[";
        for (unsigned int i = 0; i < before.size(); i++) {
            cout << before[i] << ((i == before.size() - 1) ? "" : ", ");
        }
        cout << "] --> [";
        for (unsigned int i = 0; i < after.size(); i++) {
            cout << after[i] << ((i == after.size() - 1) ? "" : ", ");
        }
        cout << "]" << endl;
    }
}

template <typename T>
void measure_heap_sort(const vector<T> &data, const string &data_traits = "")
{
    vector<T> tmp = data;   // Work on a copy to not destroy original content

    posix_time::ptime start = posix_time::microsec_clock::local_time();
    heap_sort(tmp.begin(), tmp.end(), std::less<T>());
    posix_time::time_duration td = posix_time::microsec_clock::local_time() - start;
    log(data_traits, "heap_sort", tmp.size(), td.total_microseconds());
    print<T>(data, tmp, "heap_sort " + data_traits + ' ');
}

template <typename T>
void measure_quick_sort(const vector<T> &data, const string &data_traits = "")
{
    vector<T> tmp = data;   // Work on a copy to not destroy original content

    posix_time::ptime start = posix_time::microsec_clock::local_time();
    quick_sort(tmp.begin(), tmp.end(), std::less<T>());
    posix_time::time_duration td = posix_time::microsec_clock::local_time() - start;
    log(data_traits, "recursive_quick_sort", tmp.size(), td.total_microseconds());
    print<T>(data, tmp, "recursive_quick_sort" + data_traits + ' ');
}

template <typename T>
void measure_std_sort(const vector<T> &data, const string &data_traits = "")
{
    vector<T> tmp = data;   // Work on a copy to not destroy original content

    posix_time::ptime start = posix_time::microsec_clock::local_time();
    std::sort(tmp.begin(), tmp.end());
    posix_time::time_duration td = posix_time::microsec_clock::local_time() - start;
    log(data_traits, "std_sort", tmp.size(), td.total_microseconds());
    print<T>(data, tmp, "std_sort" + data_traits + ' ');
}

template <typename T>
void measure_std_partial_sort(const vector<T> &data, const string &data_traits = "")
{
    vector<T> tmp = data;   // Work on a copy to not destroy original content

    posix_time::ptime start = posix_time::microsec_clock::local_time();
    std::partial_sort(tmp.begin(), tmp.end(), tmp.end());
    posix_time::time_duration td = posix_time::microsec_clock::local_time() - start;
    log(data_traits, "std_partial_sort", tmp.size(), td.total_microseconds());
    print<T>(data, tmp, "std_partial_sort" + data_traits + ' ');
}

template <typename T>
void measure_std_stable_sort(const vector<T> &data, const string &data_traits = "")
{
    vector<T> tmp = data;   // Work on a copy to not destroy original content

    posix_time::ptime start = posix_time::microsec_clock::local_time();
    std::stable_sort(tmp.begin(), tmp.end());
    posix_time::time_duration td = posix_time::microsec_clock::local_time() - start;
    log(data_traits, "std_stable_sort", tmp.size(), td.total_microseconds());
    print<T>(data, tmp, "std_stable_sort" + data_traits + ' ');
}

template <typename T>
void measure_split_sort(const vector<T> &data, const string &data_traits = "")
{
    vector<T> tmp = data;   // Work on a copy to not destroy original content

    posix_time::ptime start = posix_time::microsec_clock::local_time();
    split_sort(tmp.begin(), tmp.end(), std::less<T>());
    posix_time::time_duration td = posix_time::microsec_clock::local_time() - start;
    log(data_traits, "split_sort", tmp.size(), td.total_microseconds());
    print<T>(data, tmp, "split_sort" + data_traits + ' ');
}

template <typename T>
void measure_bubble_ext_sort(const vector<T> &data, const string &data_traits = "")
{
    vector<T> tmp = data;   // Work on a copy to not destroy original content

    posix_time::ptime start = posix_time::microsec_clock::local_time();
    bubble_ext_sort(tmp.begin(), tmp.end(), std::less<T>());
    posix_time::time_duration td = posix_time::microsec_clock::local_time() - start;
    log(data_traits, "bubble_ext_sort", tmp.size(), td.total_microseconds());
    print<T>(data, tmp, "bubble_ext_sort" + data_traits + ' ');
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
        ("theads", program_options::value<unsigned int>(&thread_count)->default_value(thread::hardware_concurrency()), "set worker thead count")
        ("verbose", "display extra runtime output")
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
    if (vm.count("verbose")) {
        g_verbose = true;
    }
    cout << "using " << thread_count << " threads and a data size range from " << min_size << " to " << max_size << endl;

    // Create current log directory path
    g_log_dir = filesystem::complete("logs/", filesystem::current_path());
    filesystem::create_directory(g_log_dir);
    string now = lexical_cast<string>(boost::posix_time::second_clock::local_time());
    replace_all(now, " ", "_");
    g_log_dir = filesystem::complete(now, g_log_dir);
    filesystem::create_directory(g_log_dir);
    if (!filesystem::exists(g_log_dir)) {
        cerr << "unable to create log directory " << g_log_dir << endl;
        return 1;
    } else {
        cout << "created log directory " << g_log_dir << endl;
    }

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
            cout << "measure " << data_names[j] << " data..." << endl;
            tp.schedule(bind(measure_heap_sort<int>, data[j], data_names[j]));
            //tp.schedule(bind(measure_quick_sort<int>, data[j], data_names[j]));
            tp.schedule(bind(measure_std_sort<int>, data[j], data_names[j]));
            tp.schedule(bind(measure_std_partial_sort<int>, data[j], data_names[j]));
            tp.schedule(bind(measure_std_stable_sort<int>, data[j], data_names[j]));
            tp.schedule(bind(measure_split_sort<int>, data[j], data_names[j]));
            tp.schedule(bind(measure_bubble_ext_sort<int>, data[j], data_names[j]));
        }
    }
    return 0;
}
