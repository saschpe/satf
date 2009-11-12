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

#ifndef SORTRUNNER_H
#define SORTRUNNER_H

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QMutexLocker>
#include <QRunnable>
#include <QString>
#include <QVector>


template <class RandomAccessIterator, class Less>
class SortRunner : public QRunnable
{
public:
    SortRunner(QVector<int> data, const QString &name)
        : m_data(data), m_name(name) {
    }

    /**
     * Explicitly non-threadsafe method, use only when no SortRunner
     * thread instances are running.
     */
    static void init(const QString &logDirPrefix = QString()) {
        s_logDirName = "logs/" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss") + '/' + logDirPrefix + '/';

        // Check if log directory exists and create it accordingly.
        if (!QDir(s_logDirName).exists()) {
            if (!QDir::current().mkpath(s_logDirName)) {
                qWarning() << "Unable to create log directory" << s_logDirName;
                return;
            } else {
                qDebug() << "Created log directory" << s_logDirName;
            }
        }
    }

private:
    bool less(int x, int y) {
        return x < y;
    }

    virtual void run() {
        QTime m_time;
        m_time.start();
        m_func(m_data.begin(), m_data.end(), less);
        int time = m_time.elapsed();

        // Open the log file for appending and write the tuple "size", "time" as CSV
        QMutexLocker locker(&s_mutex);
        QFile logFile(s_logDirName + '/' + m_name);
        if (!logFile.open(QFile::WriteOnly | QFile::Append)) {
            qWarning() << "Unable to open log file" << logFile.fileName();
            return;
        }
        logFile.write(QByteArray::number(m_data.size()) + ' ' +
                      QByteArray::number(time) + '\n');
        logFile.close();
    }

    void (*m_func)(RandomAccessIterator, RandomAccessIterator, Less);
    QVector<int> m_data;
    const QString m_name;

    static QMutex s_mutex;
    static QString s_logDirName;
};

#endif
