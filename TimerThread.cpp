/*
 * Copyright 2014 Takashi Inoue
 *
 * This file is part of Slider.
 *
 * Slider is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Slider is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Slider.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TimerThread.h"
#include <QElapsedTimer>
#include <QReadLocker>
#include <QWriteLocker>
#include <QDebug>

TimerThread::TimerThread(QObject *parent) :
    QThread(parent),
    msecPerFrame(1000.0 / 60.0)
{
}

void TimerThread::stop()
{
    rwlock.lockForWrite();
    stopFlag = true;
    rwlock.unlock();

    qDebug() << "TimerThread : Stop requested";

    operationMutex.tryLock();
    waitFinishOperation.wakeAll();
    operationMutex.unlock();
}

void TimerThread::setFPS(double fps)
{
    QWriteLocker locker(&rwlock);

    msecPerFrame = 1000.0 / fps;
}

void TimerThread::onOperationFinished()
{
//    qDebug() << "TimerThread : FinishOperation";
    waitFinishOperation.wakeAll();
    operationMutex.unlock();
}

void TimerThread::run()
{
    qDebug() << "TimerThread : Start" << currentThreadId();

    QElapsedTimer timer;

    do {
        timer.start();

        operationMutex.lock();

//        qDebug() << "TimerThread : Tick";
        emit tick(&operationMutex);
//        qDebug() << "TimerThread : Wait";
        waitFinishOperation.wait(&operationMutex);
//        qDebug() << "TimerThread : Wake";

        operationMutex.unlock();

        rwlock.lockForRead();
        qint64 waitTime = msecPerFrame - timer.elapsed();
        rwlock.unlock();

//        qDebug() << waitTime;

        if (waitTime > 3)
            msleep(waitTime);
    } while(!isStopped());

    qDebug() << "TimerThread : Finish";
}

bool TimerThread::isStopped()
{
    QReadLocker locker(&rwlock);

    return stopFlag;
}
