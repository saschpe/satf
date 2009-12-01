/*
    Copyright (c) 2009 Hagen Schink <hschink@st.ovgu.de>

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

#ifndef SPLITSORT_H
#define SPLITSORT_H

template <class RandomAccessIterator, class Less>
void split_sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type type;

    std::vector<type> xu, xs, xg, merge;
    RandomAccessIterator pos(first);

    xu.push_back(*first);
    ++pos;

    while (pos != beyond) {
        if (xu.empty()) {
            xu.push_back(*pos);
        } else if (less(*(xu.end() - 1), *pos)) {
            xu.push_back(*pos);
        } else {
            xg.push_back(*(xu.end()));
            xu.pop_back();
            xs.push_back(*pos);
        }

        ++pos;
    }

    if (xs.size() > 1) {
        split_sort(xs.begin(), xs.end(), less);
    }
    if (xg.size() > 1) {
        split_sort(xg.begin(), xg.end(), less);
    }
    if (xs.size() + xg.size () > 0) {
        copy (xg.begin(), xg.end(), back_inserter(xs));
        inplace_merge (xs.begin(), xs.begin() + xg.size(), xs.end(), less);
    }

    copy(xu.begin(), xu.end(), first);
    copy(xs.begin(), xs.end(), first + xu.size());
    inplace_merge(first, first + xu.size(), beyond, less);
}

template <class InputIterator, class OutputIterator, class Less>
OutputIterator split_sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less)
{
    typedef typename std::iterator_traits<InputIterator>::value_type type;
    std::vector<type> values;
    std::copy(first, beyond, back_inserter(values));
    split_sort(values.begin(), values.end(), less);
    return std::copy(values.begin(), values.end(), result);
}

#endif
