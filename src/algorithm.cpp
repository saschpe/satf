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
#include <QFile>

// Set module-private algorithm log directory path
static const QString LOG_DIR = "logs/log-" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");

Algorithm::Algorithm(QVariantList data)
    : QObject(), m_data(data)
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

    // Check if log directory exists or create it. Furthermore
    // open the log file for appending.
    m_logMutex.lock();
    if (!QDir(LOG_DIR).exists() && !QDir::current().mkpath(LOG_DIR)) {
        m_logMutex.unlock();
        qDebug() << "Unable to create log directory" << LOG_DIR;
        return;
    }
    QFile logFile(LOG_DIR + '/' + metaObject()->className());
    if (!logFile.open(QFile::WriteOnly | QFile::Append)) {
        m_logMutex.unlock();
        qDebug() << "Unable to open log file" << logFile.fileName();
        return;
    }

    // Finally log tuple "size, time" as CSV and flush/close the file
    logFile.write(QByteArray::number(m_data.size()) + ',' +
                  QByteArray::number(time) + '\n');
    logFile.close();
    m_logMutex.unlock();
}
