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

#include "ThreadOperation.h"

#include <QReadLocker>
#include <QWriteLocker>
#include <QDebug>

ThreadOperation::ThreadOperation(QObject *parent)
    : QObject(parent)
    , suspended(false)
    , stopped(false)
{
    connect(this, &ThreadOperation::startOperation, this, &ThreadOperation::exec);
}

ThreadOperation::~ThreadOperation()
{
    stop();
    wait();
}

void ThreadOperation::stop()
{
    if (!isRunning()) {
        qDebug() << className() << "skip request to stop";
        return;
    }

    QWriteLocker locker(&rwLockForThreadOperation);

    qDebug() << className() << "request to stop";

    stopped = true;
}

void ThreadOperation::start()
{
    emit startOperation();
}

void ThreadOperation::wait()
{
    qDebug() << className() << "wait...";

    QMutexLocker lockerForSync(&mutexRunning);

    qDebug() << className() << "wake";
}

void ThreadOperation::suspend()
{
    qDebug() << className() << "request to suspend";

    QMutexLocker lockerForSuspend(&mutexSuspend);

    suspended = true;
}

void ThreadOperation::resume()
{
    qDebug() << className() << "resume";

    QMutexLocker lockerForSuspend(&mutexSuspend);

    suspended = false;
    waitSuspend.wakeAll();
}

bool ThreadOperation::isRunning()
{
    if (mutexRunning.tryLock()) {
        mutexRunning.unlock();

        qDebug() << className() << "is not runnning";

        return false;
    }

    qDebug() << className() << "is runnning";

    return true;
}

QString ThreadOperation::className() const
{
    return "ThreadOperation";
}

bool ThreadOperation::isStopped() const
{
    QReadLocker locker(&rwLockForThreadOperation);

    return stopped;
}

void ThreadOperation::exec()
{
    QMutexLocker lockerForSync(&mutexRunning);

    emit started();

    qDebug() << className() << "is started";

    execImpl();

    if (!isStopped()) {
        emit completed();

        qDebug() << className() << "is completed";

        return;
    }

    emit aborted();

    qDebug() << className() << "is aborted";

    QWriteLocker locker(&rwLockForThreadOperation);

    stopped = false;

    qDebug() << className() + "::stopped" << stopped;
}
