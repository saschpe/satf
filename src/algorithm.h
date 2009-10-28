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

#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QFile>
#include <QMutex>
#include <QRunnable>
#include <QVariant>

/**
 * Abstract (pure virtual) threaded base class for generic
 * algorithms. Provides runtime measurement and logging.
 */
class Algorithm : public QObject, public QRunnable
{
    Q_OBJECT

public:
    /**
     * The standard constructor takes data by-value since the original
     * content of 'data' should remain unchanged.
     */
    Algorithm(QVariantList data);
    virtual ~Algorithm();

    /**
     * This method is not thread-safe, only use it before instancing
     * any subclass.
     */
    static void setLogPath(const QString &path);

protected:
    /**
     * Implement this method in subclasses to do the actual work.
     */
    virtual void compute() = 0;

    QVariantList m_data;

private:
    virtual void run();

    bool m_logEnabled;
    QMutex m_logMutex;
    QFile m_logFile;

    static QString s_logPath;
};

#endif
