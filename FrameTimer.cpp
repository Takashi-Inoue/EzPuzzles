#include "FrameTimer.h"

#include <QElapsedTimer>
#include <QThread>
#include <QDebug>

FrameTimer::FrameTimer(double fps) :
    msecsPerFrame(1000.0 / fps)
{
    Q_ASSERT(fps > 0);
}

void FrameTimer::stop()
{
    ThreadOperation::stop();

    mutexOperation.tryLock();
    waitOperation.wakeAll();
    mutexOperation.unlock();
}

QString FrameTimer::className() const
{
    return "FrameTimer";
}

void FrameTimer::execImpl()
{
    qDebug() << "FrameTimer ThreadID" << thread()->currentThreadId();

    QElapsedTimer timer;

    do {
        timer.start();

        mutexOperation.lock();

//        qDebug() << "TimerThread : Tick";
        emit tick(&mutexOperation, &waitOperation);
//        qDebug() << "TimerThread : Wait";
        waitOperation.wait(&mutexOperation);
//        qDebug() << "TimerThread : Wake";

        mutexOperation.unlock();

        if (isStopped())
            break;

        int waitTime = msecsPerFrame - timer.elapsed();

//        qDebug() << waitTime;

        if (waitTime > 0)
            QThread::msleep(waitTime);
    } while(!isStopped());

    qDebug() << "TimerThread : Finish";
}
