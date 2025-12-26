#include <QCoreApplication>
#include <QThread>
#include <QThreadPool>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>
#include <QSemaphore>
#include <QTimer>
#include <QDebug>
#include <QAtomicInt>

class WorkerThread : public QThread
{
    Q_OBJECT

public:
    WorkerThread(int id, QObject *parent = nullptr) : QThread(parent), workerId(id) {}

protected:
    void run() override
    {
        qDebug() << "Worker thread" << workerId << "started";
        
        for (int i = 0; i < 5; ++i) {
            msleep(100);
            qDebug() << "Worker" << workerId << "processing item" << i;
        }
        
        qDebug() << "Worker thread" << workerId << "finished";
    }

private:
    int workerId;
};

class TaskRunnable : public QRunnable
{
public:
    TaskRunnable(int taskId) : id(taskId) {}
    
    void run() override
    {
        qDebug() << "Task" << id << "running in thread pool";
        QThread::msleep(50);
        qDebug() << "Task" << id << "completed";
    }

private:
    int id;
};

class Producer : public QThread
{
    Q_OBJECT

public:
    Producer(QMutex *mutex, QWaitCondition *notEmpty, QWaitCondition *notFull, 
             QList<int> *buffer, int *count, QObject *parent = nullptr)
        : QThread(parent), m_mutex(mutex), m_notEmpty(notEmpty), m_notFull(notFull), 
          m_buffer(buffer), m_count(count) {}

protected:
    void run() override
    {
        for (int i = 0; i < 5; ++i) {
            QMutexLocker locker(m_mutex);
            
            // Wait if buffer is full (capacity of 3)
            while (*m_count >= 3) {
                m_notFull->wait(m_mutex);
            }
            
            // Produce item
            m_buffer->append(i);
            (*m_count)++;
            qDebug() << "Producer: produced" << i << "(buffer size:" << *m_count << ")";
            
            // Signal that buffer is not empty
            m_notEmpty->wakeOne();
            
            // Release lock temporarily to allow consumer to work
            locker.unlock();
            msleep(50);
        }
    }

private:
    QMutex *m_mutex;
    QWaitCondition *m_notEmpty;
    QWaitCondition *m_notFull;
    QList<int> *m_buffer;
    int *m_count;
};

class Consumer : public QThread
{
    Q_OBJECT

public:
    Consumer(QMutex *mutex, QWaitCondition *notEmpty, QWaitCondition *notFull,
             QList<int> *buffer, int *count, QObject *parent = nullptr)
        : QThread(parent), m_mutex(mutex), m_notEmpty(notEmpty), m_notFull(notFull),
          m_buffer(buffer), m_count(count) {}

protected:
    void run() override
    {
        for (int i = 0; i < 5; ++i) {
            QMutexLocker locker(m_mutex);
            
            // Wait if buffer is empty
            while (*m_count == 0) {
                m_notEmpty->wait(m_mutex);
            }
            
            // Consume item
            int item = m_buffer->takeFirst();
            (*m_count)--;
            qDebug() << "Consumer: consumed" << item << "(buffer size:" << *m_count << ")";
            
            // Signal that buffer is not full
            m_notFull->wakeOne();
            
            // Release lock temporarily
            locker.unlock();
            msleep(75);
        }
    }

private:
    QMutex *m_mutex;
    QWaitCondition *m_notEmpty;
    QWaitCondition *m_notFull;
    QList<int> *m_buffer;
    int *m_count;
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "Starting Advanced Threading Test";
    
    // Test 1: Basic QThread
    qDebug() << "\n=== Test 1: Basic QThread ===";
    QList<WorkerThread*> workers;
    for (int i = 0; i < 3; ++i) {
        WorkerThread *worker = new WorkerThread(i);
        workers.append(worker);
        worker->start();
    }
    
    for (WorkerThread *worker : workers) {
        worker->wait();
        delete worker;
    }
    
    // Test 2: QThreadPool
    qDebug() << "\n=== Test 2: QThreadPool ===";
    QThreadPool *pool = QThreadPool::globalInstance();
    qDebug() << "Thread pool max threads:" << pool->maxThreadCount();
    
    for (int i = 0; i < 10; ++i) {
        TaskRunnable *task = new TaskRunnable(i);
        pool->start(task);
    }
    
    pool->waitForDone();
    qDebug() << "All thread pool tasks completed";
    
    // Test 3: QMutex and QWaitCondition
    qDebug() << "\n=== Test 3: Producer-Consumer with Mutex ===";
    QMutex mutex;
    QWaitCondition notEmpty;
    QWaitCondition notFull;
    QList<int> buffer;
    int bufferCount = 0;
    
    Producer producer(&mutex, &notEmpty, &notFull, &buffer, &bufferCount);
    Consumer consumer(&mutex, &notEmpty, &notFull, &buffer, &bufferCount);
    
    producer.start();
    consumer.start();
    
    producer.wait();
    consumer.wait();
    
    qDebug() << "Producer-Consumer test completed";
    
    // Test 4: QSemaphore
    qDebug() << "\n=== Test 4: QSemaphore ===";
    QSemaphore semaphore(3);
    
    class SemaphoreTask : public QRunnable
    {
    public:
        SemaphoreTask(int id, QSemaphore *sem) : taskId(id), semaphore(sem) {}
        
        void run() override
        {
            semaphore->acquire();
            qDebug() << "Task" << taskId << "acquired semaphore";
            QThread::msleep(100);
            qDebug() << "Task" << taskId << "releasing semaphore";
            semaphore->release();
        }
        
    private:
        int taskId;
        QSemaphore *semaphore;
    };
    
    for (int i = 0; i < 8; ++i) {
        pool->start(new SemaphoreTask(i, &semaphore));
    }
    
    pool->waitForDone();
    qDebug() << "Semaphore test completed";
    
    // Test 5: QAtomicInt
    qDebug() << "\n=== Test 5: QAtomicInt ===";
    QAtomicInt atomicCounter(0);
    
    class AtomicTask : public QRunnable
    {
    public:
        AtomicTask(QAtomicInt *counter) : atomicCounter(counter) {}
        
        void run() override
        {
            for (int i = 0; i < 1000; ++i) {
                atomicCounter->fetchAndAddOrdered(1);
            }
        }
        
    private:
        QAtomicInt *atomicCounter;
    };
    
    for (int i = 0; i < 5; ++i) {
        pool->start(new AtomicTask(&atomicCounter));
    }
    
    pool->waitForDone();
    qDebug() << "Atomic counter final value:" << atomicCounter.loadAcquire();
    qDebug() << "Expected value: 5000";
    
    qDebug() << "\nAdvanced Threading Test completed successfully";
    
    return 0;
}