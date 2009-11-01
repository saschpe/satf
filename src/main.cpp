/*
   Sort Algorithm Test Fest
   Copyright (C) 2009 Sascha Peilicke <sasch.pe@gmx.de>

   Simple framework to profile different sorting algorithms.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mergesort.h"
#include "quicksort.h"

#include <QCoreApplication>
#include <QThreadPool>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QVariantList data;

    // Iterate over test data with variying size to profile algorithms
    for (unsigned int size = 1; size < 10; size++) {
        // Add another random value to the test data
        data.append(qrand());

        QThreadPool::globalInstance()->start(new QuickSort(data));
        QThreadPool::globalInstance()->start(new MergeSort(data));
        // Add more algorithms here if you have more
    }
    return 0;
}
