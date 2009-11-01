/*
    Copyright (c) 2009 Sascha Peilicke <sasch.pe@gmx.de>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
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
    for (unsigned int i = 1; i <= max_size; i++) {
        // Add another random value to the test data
        data.append(qrand());
        if (i < min_size) {
            continue;
        }

        // Add more algorithms here if you have more
        QThreadPool::globalInstance()->start(new QuickSort(data));
        QThreadPool::globalInstance()->start(new MergeSort(data));
    }
    return 0;
}
