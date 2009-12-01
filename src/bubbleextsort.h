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

#ifndef BUBBLEEXTSORT_H
#define BUBBLEEXTSORT_H

template <class InputIterator, class OutputIterator, class Less>
OutputIterator bubble_ext_sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less)
{
    typedef typename std::iterator_traits<InputIterator>::value_type type;
    std::vector<type> values;
    std::copy(first, beyond, back_inserter(values));
    bubble_ext_sort(values.begin(), values.end(), less);
    return std::copy(values.begin(), values.end(), result);
}

template <class RandomAccessIterator, class Less>
void bubble_ext_sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type type;

    RandomAccessIterator pos, pos_e;
    bool sorted = false;
    int x = 0;

    while (!sorted) {
        sorted = true;
        pos = first;
        pos_e = beyond - 1 - x;

        while (pos + 1 != beyond - x) {
            if (!less(*pos, *(pos + 1))) {
                type temp = *pos;
                *pos = *(pos + 1);
                *(pos + 1) = temp;
                sorted = false;
            }
            if (!less(*(pos_e - 1), *pos_e)) {
                type temp = *pos_e;
                *pos_e = *(pos_e - 1);
                *(pos_e - 1) = temp;
                sorted = false;
            }

            ++pos;
            --pos_e;
        }
        x++;
    }
}

#endif
