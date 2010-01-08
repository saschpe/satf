#ifndef COOKXXXSORT_H
#define COOKXXXSORT_H

#include <algorithm>
#include <vector>

template <typename RandomAccessIterator, typename Less>
void cook_xxx_sort(RandomAccessIterator start, RandomAccessIterator beyond, Less less)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    typedef typename std::vector<value_type>::const_iterator const_vector_iterator;

    std::vector<value_type> others, sorted;
    others.reserve(beyond - start);
    sorted.reserve(beyond - start);

    for (const_vector_iterator iter = start; iter != beyond; ++iter) {
        if (sorted.empty() || less(sorted.back(), *iter)) {
            sorted.push_back(*iter);
        } else {
            others.push_back(*iter);
            others.push_back(sorted.back());
            sorted.pop_back();
        }
    }
    std::sort(others.begin(), others.end(), less);
    std::merge(others.begin(), others.end(), sorted.begin(), sorted.end(), start);
}

template<class InputIterator, class OutputIterator, class Less>
OutputIterator cook_xxx_sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less)
{
    typedef typename InputIterator::value_type value_type;

    std::vector<value_type> data;
    data.reserve(512);

    std::copy(first, beyond, std::back_inserter(data));
    cook_xxx_sort(data.begin(), data.end(), less);
    return std::copy(data.begin(), data.end(), result);
}

#endif // COOKXXXSORT_H
