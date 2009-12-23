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

#ifndef QUICKSORT_H
#define QUICKSORT_H

// Slightly adapted the version found here:
// http://en.wikibooks.org/wiki/Algorithm_implementation/Sorting/Quicksort#C.2B.2B

template <class RandomAccessIterator, class Less>
void quick_sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less)
{
    if (first != beyond) {
        RandomAccessIterator middle = std::partition(first, beyond, std::bind2nd(less, *first));
        quick_sort(first, middle, less);
        quick_sort(std::max(first + 1, middle), beyond, less);
    }
}

template <class InputIterator, class OutputIterator, class Less>
OutputIterator quick_sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less)
{
    typedef typename std::iterator_traits<OutputIterator>::value_type value_type;
    std::vector<value_type> tmp(first, beyond);
    InputIterator begin = tmp.begin();
    InputIterator end = tmp.end();
    if (begin != end) {
        InputIterator middle = std::partition(begin, end, std::bind2nd(less, *begin));
        quick_sort(begin, middle, less);
        quick_sort(std::max(begin + 1, middle), end, less);
    }
    std::copy(tmp.begin(), tmp.end(), result);
    return result;
}

#endif
