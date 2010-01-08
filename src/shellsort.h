#ifndef SHELLSORT_H
#define SHELLSORT_H

#include <vector>

template<class InputIterator, class OutputIterator, class Less>
OutputIterator shell_sort(InputIterator first, InputIterator beyond, OutputIterator result, Less less)
{
    if (beyond == first || beyond == ++first) {       //std::liste leer oder einelementig?
        return first;
    }

    // prev und begin werden fuer Vergleich auf Sortiertheit gebraucht
    InputIterator prev(--first);
    InputIterator tmp;
    InputIterator begin(first);
    ++begin;

    typedef typename std::iterator_traits<InputIterator>::value_type elemType;
    typedef typename std::vector<elemType> vec;

    vec to_sort;
    to_sort.push_back(*first);

    int sorted = 1;        //Zeigt an, ob Folge sortiert
    int length = 1;

    //zu sortierende Elemente in einen std::vector kopieren
    for (tmp = begin; tmp != beyond; ++tmp) {
        to_sort.push_back(*tmp);
        if (!less(*prev, *tmp)) {
            sorted = 0;
        }
        ++prev;
        ++length;
    }

    if (sorted == 1) {
        return copy(to_sort.begin(), to_sort.end(), result);
    }

    /*
     *ShellSort
     */
    typedef typename vec::iterator vecIter;
    elemType key;
    vecIter iter_for;
    vecIter iter_while(to_sort.begin());
    int k, h;
    int distance[16] = { 1391376, 463792, 198768, 86961, 33936,
                         13776, 4592, 1968, 861, 336,
                         112, 48, 21, 7, 3, 1 };

    for (k = 0; k < 16; k++) {
        h = distance[k];
        iter_for = to_sort.begin();
        for (iter_for += h; iter_for < to_sort.end(); iter_for++) {
            key = *iter_for;
            iter_while =  iter_for;

            // Anfang_der_Liste_+_h definieren
            vecIter start(to_sort.begin());
            start = start + h;

            /*
             * suche nach der richtigen Stelle zum Einfuegen in den bereits sortierten Bereich:
             * Solange Vorgaenger_mit_Abstand_h groesser ist als das Element, vertausche Element mit Vorgaenger.
             * Beachte dabei, dass nicht ueber den Anfang_der_Liste_+_h hinaus iteriert wird.
             */
            while (iter_while >= start && less(key, *(iter_while - h))) {
                *iter_while = *(iter_while - h);
                iter_while = iter_while - h;
            }
            *iter_while = key;
        }
    }
    return copy(to_sort.begin(), to_sort.end(), result);
}

template <class RandomAccessIterator, class Less>
void shell_sort(RandomAccessIterator first, RandomAccessIterator beyond, Less less)
{
    shell_sort(first, beyond, first, less);
}

#endif // SHELLSORT_H
