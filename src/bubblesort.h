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

#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include "algorithm.h"

class BubbleSort : public Algorithm
{
public:
    BubbleSort(QVariantList data) : Algorithm(data, "BubbleSort") {}

private:
    void compute() {
        for (int i = 0; i < m_data.size(); i++) {
            for (int j = i; j < m_data.size(); j++) {
                // Arbitrary comparison here
                if (m_data[j].toInt() < m_data[i].toInt()) {
                    QVariant tmp = m_data[i];
                    m_data[i] = m_data[j];
                    m_data[j] = tmp;
                }
            }
        }
    }
};

#endif

