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
#include <QDebug>
#include <QThreadPool>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QVariantList data;
    unsigned int min_size = 1, max_size = 1000;

    // Check for commandline arguments
    if (argc == 2) {
        max_size = QString(argv[1]).toUInt();
    } else if (argc == 3) {
        min_size = QString(argv[1]).toUInt();
        max_size = QString(argv[2]).toUInt();
    } else {
        qDebug() << "Hint: You can provide a custom test data size range:\n\n"
                 << "    " << argv[0] << "[[MIN_SIZE] MAX_SIZE]\n";
    }

    qDebug() << "Using" << QThreadPool::globalInstance()->maxThreadCount()
             << "threads with a test data size range from" << min_size
             << "to" << max_size;

    // Iterate over test data with varying size to profile algorithms
    // under changing conditions. 'data' is passed by-value and thus
    // remains unchanged for easy reuse.
    for (; min_size <= max_size; min_size++) {
        // Add another random value to the test data
        data.append(qrand());

        // Add more algorithms here if you have more
        QThreadPool::globalInstance()->start(new QuickSort(data));
        QThreadPool::globalInstance()->start(new MergeSort(data));
    }
    return 0;
}
