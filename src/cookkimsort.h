#ifndef COOKKIMSORT_H
#define COOKKIMSORT_H

#include <algorithm>
#include <vector>

template <typename InputIterator, typename OutputIterator, typename Less>
OutputIterator cook_kim_sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less)
{
    typedef typename std::iterator_traits<InputIterator>::value_type T;

    std::vector<T> xu, xo;

    while (first != beyond) {
        if (xu.empty()) {
            xu.push_back(*first);
        } else if (less(xu.back(), *first)) {
            xu.push_back(*first);
        } else {
            xo.push_back(*first);
            xo.push_back(xu.back());
            xu.pop_back();
        }
        ++first;
    }

    if (xo.size() > 1) {
        std::sort(xo.begin(), xo.end(), less);
    }
    return std::merge(xu.begin(), xu.end(), xo.begin(), xo.end(), result);
}

template <class RandomAccessIterator, class Less>
void cook_kim_sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less)
{
    cook_kim_sort(first, beyond, first, less);
}

#endif // COOKKIMSORT_H
