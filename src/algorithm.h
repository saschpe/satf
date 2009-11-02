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

#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QRunnable>
#include <QVariant>

/**
 * Abstract (pure virtual) threaded base class for generic algorithms.
 * Provides runtime measurement and logging.The log file content is not
 * guaranteed to be in sort order.
 */
class Algorithm : public QRunnable
{
public:
    /**
     * The standard constructor takes data by-value since the original
     * content of 'data' should remain unchanged.
     */
    Algorithm(QVariantList data, const char *name);
    virtual ~Algorithm();

protected:
    /**
     * Implement this method in subclasses to do the actual work.
     */
    virtual void compute() = 0;

    QVariantList m_data;

private:
    virtual void run();

    const QLatin1String m_name;
};

#endif
