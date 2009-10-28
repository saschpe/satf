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

#include <QDebug>
#include <QTime>

QString Algorithm::s_logPath = "logs/";

Algorithm::Algorithm(QVariantList data)
    : QObject(), m_data(data), m_logEnabled(false)
    , m_logFile(s_logPath + '/' + metaObject()->className())
{
    // Open a logfile for all instances of an algorithmn subclass
    if (m_logFile.open(QFile::WriteOnly | QFile::Append)) {
        m_logEnabled = true;
        qDebug() << "Logging enabled for" << this->metaObject()->className();
        m_logFile.write("size,time\n");
    }
}

Algorithm::~Algorithm()
{
    if (m_logEnabled) {
        m_logFile.close();
    }
}

void Algorithm::setLogPath(const QString &path)
{
    s_logPath = path;
}

void Algorithm::run()
{
    QTime m_time;

    m_time.start();
    compute();
    int time = m_time.elapsed();
    if (m_logEnabled) {
        m_logMutex.lock();
        // Log tuple "size, time" as CSV
        m_logFile.write(QByteArray::number(m_data.size()) + ',' +
                        QByteArray::number(time) + '\n');
        m_logMutex.unlock();
    }
}
