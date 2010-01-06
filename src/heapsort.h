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

#ifndef HEAPSORT_H
#define HEAPSORT_H

template <class RandomAccessIterator, class Less>
void heap_sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less)
{
    std::make_heap(first, beyond, less);
    std::sort_heap(first, beyond, less);
}

template <class InputIterator, class OutputIterator, class Less>
OutputIterator heap_sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less )
{
    typedef typename std::iterator_traits<OutputIterator>::value_type value_type;
    std::vector<value_type> tmp(first, beyond);
    std::make_heap(tmp.begin(), tmp.end(), less);
    std::sort_heap(tmp.begin(), tmp.end(), less);
    return std::copy(tmp.begin(), tmp.end(), result);
}

#endif

