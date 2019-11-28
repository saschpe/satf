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

#ifndef COUNTLESS_H
#define COUNTLESS_H

#include <boost/thread.hpp>

/**
 * This class is a custom comparator and uses an ugly hack to overcome a
 * design-error in the STL. Algorithms like std::sort and std::stable_sort
 * take a Compare object by-value, which is useless for us if we want to
 * count the calls to CountLess::operator()(). Therefore we make the counting
 * variable a thread-local static so that we have at least a sane value.
 *
 * Note that the counter is resetted once CountLess::count() is called, so
 * you can retrieve the value only once.
 */
template <typename T>
class CountLess
{
public:
    CountLess() {
        if (!s_count.get()) {
            s_count.reset(new unsigned int(0));
        }
    }

    static unsigned int count() {
        unsigned int ret = *s_count;
        *s_count = 0;
        return ret;
    }

    bool operator()(T a, T b) {
        (*s_count)++;
        return a < b;
    }

private:
    static boost::thread_specific_ptr<unsigned int> s_count;
};

// Don't forget to allocate physical space in the binary...
template <typename T>
boost::thread_specific_ptr<unsigned int> CountLess<T>::s_count;

#endif // COUNTLESS_H
