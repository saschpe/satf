/*
    Copyright 2009 Sascha Peilicke <sasch.pe@gmx.de>

    Simple framework to measure the runtime cost of
    different sorting algorithms.
 */

#include <QCoreApplication>
#include <QDebug>
#include <QThreadPool>
#include <QTime>
#include <QVector>

/**
 * Abstract (pure virtual) threaded base class for generic
 * algorithms. Provides runtime measurement and logging.
 */
template <typename T>
class Algorithm : public QObject, public QRunnable
{
public:
    Algorithm(const QVector<T> &data) : m_data(data) {}
    virtual ~Algorithm() {}

    void run() {
        m_time.start();
        compute();
        int time = m_time.elapsed();
        qDebug() << objectName() << "took" << time << "milliseconds to sort" << m_data.size() << "item(s).";
    }

protected:
    virtual void compute() = 0;

    QVector<T> m_data;

private:
    QTime m_time;
};

template <typename T>
class QuickSort : public Algorithm<T>
{
public:
    QuickSort(const QVector<T> &data) : Algorithm<T>(data) {}

private:
    void compute() {
    }
};

template <typename T>
class MergeSort : public Algorithm<T>
{
public:
    MergeSort(const QVector<T> &data) : Algorithm<T>(data) {}

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

    // Iterate over test data with variying size to profile algorithms
    for (unsigned int size = 1; size < 2; size++) {
        qDebug() << "Generating test data with" << size << "values...";
        QVector<double> data(size);

        qDebug() << "Filling thread pool with algorithms for the test data...";
        QThreadPool::globalInstance()->start(new QuickSort<double>(data));
        QThreadPool::globalInstance()->start(new MergeSort<double>(data));
        // Add more algorithms here if you have more
    }
    return EXIT_SUCCESS;
}
