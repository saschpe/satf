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
