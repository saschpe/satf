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

#include "utility.h"

#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

static boost::filesystem::path g_log_dir;

bool log_init(const std::string &log_dir)
{
    g_log_dir = boost::filesystem::complete(log_dir, boost::filesystem::current_path());
    boost::filesystem::create_directory(g_log_dir);
    std::string now = boost::lexical_cast<std::string>(boost::posix_time::second_clock::local_time());
    boost::replace_all(now, " ", "_");
    g_log_dir = boost::filesystem::complete(now, g_log_dir);
    boost::filesystem::create_directory(g_log_dir);
    if (!boost::filesystem::exists(g_log_dir)) {
        std::cerr << "unable to create log directory " << g_log_dir << std::endl;
        return false;
    } else {
        std::cout << "created log directory " << g_log_dir << std::endl;
    }
    return true;
}

void log(const std::string &name, const std::string &data_traits, int size, unsigned int time_msecs, unsigned int comparison_count)
{
    static boost::mutex log_mutex;

    boost::lock_guard<boost::mutex> lock(log_mutex);
    boost::filesystem::path log_dir = boost::filesystem::complete(data_traits, g_log_dir);
    boost::filesystem::path log_file = boost::filesystem::complete(name, log_dir);
    boost::filesystem::create_directory(log_dir);
    boost::filesystem::ofstream ofs(log_file, std::ios_base::app | std::ios_base::out);
    ofs << size << ' ' << time_msecs << ' ' << comparison_count << std::endl;
}

template <typename T>
void print_vector(const std::vector<T> &before, const std::vector<T> &after, const std::string &line_prefix)
{
    static boost::mutex print_mutex;

    boost::lock_guard<boost::mutex> lock(print_mutex);
    std::cout << line_prefix << "[";
    for (unsigned int i = 0; i < before.size(); i++) {
        std::cout << before[i] << ((i == before.size() - 1) ? "" : ", ");
    }
    std::cout << "] --> [";
    for (unsigned int i = 0; i < after.size(); i++) {
        std::cout << after[i] << ((i == after.size() - 1) ? "" : ", ");
    }
    std::cout << "]" << std::endl;
}
