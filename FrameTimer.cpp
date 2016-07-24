/*
 * Copyright 2016 Takashi Inoue
 *
 * This file is part of EzPuzzles.
 *
 * EzPuzzles is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EzPuzzles is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EzPuzzles.  If not, see <http://www.gnu.org/licenses/>.
 */
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

        int waitTime = msecsPerFrame - timer.elapsed();

//        qDebug() << waitTime;

        if (waitTime > 0)
            thread()->msleep(waitTime); // lag on win10, not on win7. ???
    } while(!isStopped());

    qDebug() << "TimerThread : Finish";
}
