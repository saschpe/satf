#ifndef MELSORT_H
#define MELSORT_H

#include <algorithm>
#include <deque>
#include <vector>

template <typename InputIterator, typename OutputIterator, typename Less>
OutputIterator mel_sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less)
{
    typedef typename std::iterator_traits<InputIterator>::value_type T;

    if (first == beyond) {
        return result;
    }

    typedef std::deque<T> List;
    typedef std::deque<T>* Lptr;
    std::vector<Lptr> L;

    L.push_back(new List);
    L[0]->push_back(*first++);

    while (first != beyond) {
        T p = *first++;
        int i = 0;
        while (i < L.size()) {
            if (less(L[i]->back(), p)) {
                L[i]->push_back(p);
                break;
            }
            if (less(p, L[i]->front())) {
                L[i]->push_front(p);
                break;
            }
            ++i;
        }
        if (i == L.size()) {
            L.push_back(new List);
            L.back()->push_back(p);
        }
    }

    int k = L.size();
    if (k == 1) {
        result = std::copy(L[0]->begin(), L[0]->end(), result);
        delete L[0];
        return result;
    }

    Lptr Qp = new List;
    while (k > 2) {
        if (k % 2) {
            std::merge(L[k-2]->begin(), L[k-2]->end(), L[k-1]->begin(), L[k-1]->end(), std::back_inserter(*Qp));
            delete L[k-1];
            delete L[k-2];
            L[k-2] = Qp;
            Qp = new List;
        }
        if (k == 3) {
            break;
        }
        for (int j = 0; j < k / 2; ++j) {
            std::merge(L[j]->begin(), L[j]->end(), L[j+k/2]->begin(), L[j+k/2]->end(), std::back_inserter(*Qp));
            delete L[j];
            delete L[j + k / 2];
            L[j] = Qp;
            Qp = new List;
        }
        k = k / 2;
    }

    if (k == 2) {
        result = std::merge(L[0]->begin(), L[0]->end(), L[1]->begin(), L[1]->end(), result);
        delete L[0];
        delete L[1];
        return result;
    }
}

template <class RandomAccessIterator, class Less>
void mel_sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less)
{
    mel_sort(first, beyond, first, less);
}

#endif // MELSORT_H
