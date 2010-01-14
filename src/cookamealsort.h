/**
 *
 *          Copyright Matthias Walter 2010 - 2011.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See at http://www.boost.org/LICENSE_1_0.txt)
 *
 *
 *
 * @brief Adaptive sorting algorithm for sequences with one nearly ascending and one nearly descending sequence.
 * @author xammmy
 * @date 01/05/2010
 *
 */

#ifndef COOK_A_MEAL_SORT_H
#define COOK_A_MEAL_SORT_H

#include <algorithm>
#include <vector>

/**
 * Ordinary Cook Kim sort which writes unsorted elements to an output iterator.
 * 
 * @param first Iterator to the first element of a sequence with random access
 * @param beyond Iterator beyond the last element of the sequence
 * @param unsorted Iterator to write unsorted elements to
 * @param less Comparator
 * @return Iterator beyond the last element of resulting sorted range
 */

template<typename RandomAccessIterator, typename OutputIterator, typename Less>
inline RandomAccessIterator cook_kim_sort_internal (RandomAccessIterator first, RandomAccessIterator beyond,
        OutputIterator unsorted, Less less)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;

    RandomAccessIterator current = first;
    RandomAccessIterator iter;
    for (iter = first; iter != beyond; ++iter)
    {
        if (current == first || less (*(current - 1), *iter))
            *current++ = *iter;
        else
        {
            --current;
            *unsorted++ = *current;
            *unsorted++ = *iter;
        }
    }

    return current;
}

/**
 * Internal routine containing the actual sorting logic.
 * It samples some values to approximate min/max position, splits there
 * and calls ordinary Cook Kim sort for them. Afterwards, it merges everything.
 * 
 * @param first Iterator to the first element of a sequence with random access
 * @param beyond Iterator beyond the last element of the sequence
 * @param result Iterator to write resulting sequence to
 * @param less Comparator
 * @return Iterator beyond the last written element
 */

template<typename RandomAccessIterator, typename OutputIterator, typename Less>
inline OutputIterator cook_a_meal_sort_internal (RandomAccessIterator first, RandomAccessIterator beyond,
        OutputIterator result, Less less)
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    typedef typename std::reverse_iterator<RandomAccessIterator> reverse_iter_type;

    const size_t size = (beyond - first);

    if (size == 0)
        return result;

    // Sample rate
    const size_t search_skip = (size > 30) ? (beyond - first) / 16 : size / 2 + 1;

    // Search for min/max
    size_t min = 0, max = 0;
    for (size_t i = search_skip; i < size; i += search_skip)
    {
        if (less (*(first + i), *(first + min)))
            min = i;
        else if (less (*(first + max), *(first + i)))
            max = i;
    }

    std::vector<value_type> unsorted, data1, data2;
    unsorted.reserve (size);
    data1.reserve (size);
    data2.reserve (size);

    if (min < max)
    {
        // Call Cook Kim routines
        RandomAccessIterator desc1_first = cook_kim_sort_internal (reverse_iter_type (first + min), reverse_iter_type (
                first), std::back_inserter (unsorted), less).base ();
        RandomAccessIterator asc1_beyond = cook_kim_sort_internal (first + min, first + max, std::back_inserter (
                unsorted), less);
        RandomAccessIterator desc2_first = cook_kim_sort_internal (reverse_iter_type (beyond), reverse_iter_type (first
                + max), std::back_inserter (unsorted), less).base ();

        // Sort remaining
        std::sort (unsorted.begin (), unsorted.end (), less);

        // Merge
        std::merge (first + min, asc1_beyond, unsorted.begin (), unsorted.end (), std::back_inserter (data1));
        std::merge (reverse_iter_type (first + min), reverse_iter_type (desc1_first), reverse_iter_type (beyond),
                reverse_iter_type (desc2_first), std::back_inserter (data2));
    }
    else
    {
        // Call Cook Kim routines
        RandomAccessIterator asc1_beyond = cook_kim_sort_internal (first, first + max, std::back_inserter (unsorted),
                less);
        RandomAccessIterator desc1_first = cook_kim_sort_internal (reverse_iter_type (first + min), reverse_iter_type (
                first + max), std::back_inserter (unsorted), less).base ();
        RandomAccessIterator asc2_beyond = cook_kim_sort_internal (first + min, beyond, std::back_inserter (unsorted),
                less);

        // Sort remaining
        std::sort (unsorted.begin (), unsorted.end (), less);

        // Merge
        std::merge (first, asc1_beyond, first + min, asc2_beyond, std::back_inserter (data1), less);
        std::merge (reverse_iter_type (first + min), reverse_iter_type (desc1_first), unsorted.begin (),
                unsorted.end (), std::back_inserter (data2), less);
    }

    return std::merge (data1.begin (), data1.end (), data2.begin (), data2.end (), result);
}

/**
 * Cook a meal sort with STL interface for sorting on a random access sequence.
 * 
 * @param first Iterator to the first element of a sequence with random access
 * @param beyond Iterator beyond the last element of the sequence
 * @param less Comparator
 */
/// Random access interface

template<typename RandomAccessIterator, typename Less>
inline void cook_a_meal_sort (RandomAccessIterator first, RandomAccessIterator beyond, Less less)
{
    cook_a_meal_sort_internal (first, beyond, first, less);
}

/**
 * Cook a meal sort with STL interface for sorting on a sequence given by input iterators.
 * 
 * @param first Iterator to the first element of a sequence.
 * @param beyond Iterator beyond the last element of the sequence
 * @param result Iterator to write resulting sequence to
 * @param less Comparator
 * @return Iterator beyond the last written element
 */

template<class InputIterator, class OutputIterator, class Less>
inline OutputIterator cook_a_meal_sort (InputIterator first, InputIterator beyond, OutputIterator result, Less less)
{
    std::vector<typename InputIterator::value_type> data;
    data.reserve (512);
    std::copy (first, beyond, std::back_inserter (data));
    return cook_a_meal_sort_internal (data.begin (), data.end (), result, less);
}

#endif // COOK_A_MEAL_SORT_H
