#ifndef SPLAYSORT_H
#define SPLAYSORT_H

#include "splaytree.h"

#include <vector>

template <typename RandomAccessIterator, typename Less>
void splay_sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type ss1_point;

    SplayTree<ss1_point, Less> t;
    typename std::vector<ss1_point>::iterator iter;
    std::vector<ss1_point> result;

    for (iter = first; iter != beyond; iter++) {
        t.insert(*iter, less);
    }
    t.printTree();
    typename std::vector<ss1_point>::const_iterator citer;
    citer = t.toArray.begin();
    for (iter = first; iter != beyond; iter++) {
        *iter = *citer;
        citer++;
    }
}

template <typename InputIterator, typename OutputIterator, typename Less>
OutputIterator splay_sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less)
{
    typedef typename std::iterator_traits<InputIterator>::value_type ss2_point;
    std::vector<ss2_point> temp_result;
    typename std::vector<ss2_point>::iterator iter;

    for (iter = first; iter != beyond; iter++){
            temp_result.push_back(*iter);
    }
    splay_sort(temp_result.begin(), temp_result.end(), less);
    return std::copy(temp_result.begin(), temp_result.end(), result);
}

#endif // SPLAYSORT_H
