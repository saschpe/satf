/*
    Copyright (c) 2009 Sascha Peilicke <sasch.pe@gmx.de>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/

#include "mergesort.h"
#include "quicksort.h"
//#include "sortrunner.h"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
#include "boost/threadpool.hpp"

#include <algorithm>    // for std::sort()
#include <vector>
#include <iostream>

template <typename T>
bool less(T x, T y)
{
    return x < y;
}

template <typename T>
void std_sort_thread(std::vector<T> data)
{
    std::sort(data.begin(), data.end());
}

template <typename T>
void merge_sort_thread(std::vector<T> data)
{
    merge_sort(data.begin(), data.end(), less);
}

int main(int argc, char *argv[])
{
    unsigned int min_size = 1, max_size = 1000;

    // Handle command line arguments
    if (argc == 2) {
        try {
            max_size = boost::lexical_cast<unsigned int>(argv[1]);
        } catch (boost::bad_lexical_cast &) {
            std::cout << "Bad commandline argument(s) provided!" << std::endl;
        }
    } else if (argc == 3) {
        try {
            min_size = boost::lexical_cast<unsigned int>(argv[1]);
            max_size = boost::lexical_cast<unsigned int>(argv[2]);
        } catch (boost::bad_lexical_cast &) {
            std::cout << "Bad commandline argument(s) provided!" << std::endl;
        }
    } else {
        std::cout << "Hint: You can provide a custom test data size range:\n\n"
                  << "    " << argv[0] << "[[MIN_SIZE] MAX_SIZE]" << std::endl;
    }
    std::cout << "Using" << boost::thread::hardware_concurrency()
              << "threads with a test data size range from" << min_size
              << "to" << max_size << std::endl;

    // Build a random number generator
    boost::mt19937 rng;
    boost::uniform_int<> dist(min_size, max_size);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > dice(rng, dist);

    // Create our threadpool
    boost::threadpool::pool tp(boost::thread::hardware_concurrency());

    std::cout << "Measuring performance for random data..." << std::endl;
    std::vector<int> randomData;
    for (unsigned int i = min_size; i <= max_size; i++) {
        randomData.push_back(dice());

        tp.schedule(boost::bind(std_sort_thread<int>, randomData));
    }
    return 0;
}
