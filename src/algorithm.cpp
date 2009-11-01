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

#include "algorithm.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QMutexLocker>

// Set module-private log directory path
static const QString LOG_DIR = "logs/" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");
static QMutex g_mutex;

Algorithm::Algorithm(QVariantList data)
    : m_data(data)
{
}

Algorithm::~Algorithm()
{
}

void Algorithm::run()
{
    QTime m_time;
    m_time.start();
    compute();
    int time = m_time.elapsed();

    // Check if log directory exists and create it accordingly.
    QMutexLocker locker(&g_mutex);
    if (!QDir(LOG_DIR).exists()) {
        if (!QDir::current().mkpath(LOG_DIR)) {
            qWarning() << "Unable to create log directory" << LOG_DIR;
            return;
        } else {
            qDebug() << "Created log directory" << LOG_DIR;
        }
    }

    // Open the log file for appending, write the tuple "size time"
    // as CSV and flush/close the file
    QFile logFile(LOG_DIR + '/' + metaObject()->className());
    if (!logFile.open(QFile::WriteOnly | QFile::Append)) {
        qWarning() << "Unable to open log file" << logFile.fileName();
        return;
    }
    logFile.write(QByteArray::number(m_data.size()) + ' ' +
                  QByteArray::number(time) + '\n');
    logFile.close();
}
