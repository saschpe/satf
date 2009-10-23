/*
    Copyright 2009 Sascha Peilicke <sasch.pe@gmx.de>

    Sort Algorithm Test Fest

    Simple framework to profile different sorting algorithms.
 */

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMutex>
#include <QThreadPool>
#include <QTime>
#include <QVector>

/**
 * Abstract (pure virtual) threaded base class for generic
 * algorithms. Provides runtime measurement and logging.
 */
template <typename T>
class Algorithm : public QRunnable
{
public:
    explicit Algorithm(const QVector<T> &data, const QString &name) : m_data(data), m_name(name) {}
    virtual ~Algorithm() {}

    virtual void run() {
        QTime m_time;

        m_time.start();
        compute();
        int time = m_time.elapsed();
        qDebug() << m_name << "took" << time << "ms for" << m_data.size() << "item(s).";
        log(time);
    }

protected:
    virtual void compute() = 0;

    QVector<T> m_data;

private:
    void log(int time) {
        m_logMutex.lock();

        QFile file("logs/" + m_name + "-" + QByteArray::number(QCoreApplication::applicationPid()));
        if (file.open(QFile::WriteOnly | QFile::Append)) {
            // Log tuple (size, time) as CSV
            file.write(QByteArray::number(m_data.size()) + ',' + QByteArray::number(time) + '\n');
            file.close();
        }
        m_logMutex.unlock();
    }

    QMutex m_logMutex;
    QString m_name;
};

template <typename T>
class QuickSort : public Algorithm<T>
{
public:
    explicit QuickSort(const QVector<T> &data) : Algorithm<T>(data, "QuickSort") {}

private:
    void compute() {
    }
};

template <typename T>
class MergeSort : public Algorithm<T>
{
public:
    explicit MergeSort(const QVector<T> &data) : Algorithm<T>(data, "MergeSort") {}

private:
    void compute() {
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Check if the user want's to set a custom thread count
    if (argc == 2) {
        bool ok;
        int threadCount = QString(argv[1]).toInt(&ok);
        if (ok) {
            QThreadPool::globalInstance()->setMaxThreadCount(threadCount);
            qDebug() << "Set maximum algorithm thread pool size to" << threadCount;
        }
    } else if (argc >= 2) {
        qDebug() << "Usage:" << argv[0] << "[SIZE]\n";
        qDebug() << "    SIZE - set the maximum algorithm thread pool size (optional)";
        return EXIT_FAILURE;
    }

    // Create a log directory (simply fails if already present)
    QDir::current().mkdir("logs/");

    // Iterate over test data with variying size to profile algorithms
    for (unsigned int size = 1; size < 10; size++) {
        QVector<double> data(size);
        //TODO: Generate test data, change type for real application

        QThreadPool::globalInstance()->start(new QuickSort<double>(data));
        QThreadPool::globalInstance()->start(new MergeSort<double>(data));
        // Add more algorithms here if you have more
    }
    return EXIT_SUCCESS;
}

#include "main.moc"
