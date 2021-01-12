/*
 * Copyright 2021 Takashi Inoue
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

#include "ThreadFrameTimer.h"

#include <QDebug>

ThreadFrameTimer::ThreadFrameTimer(ushort fps, QObject *parent)
    : QThread(parent)
    , m_msecsPerFrame(1000.0 / fps)
{
}

void ThreadFrameTimer::stop()
{
    QWriteLocker locker(&m_lock);

    qDebug() << QStringLiteral("ThreadFrameTimer has received a stop request.");

    m_isStopRequested = true;

    m_lockForOperation.tryLockForWrite();
    m_waitOperation.wakeAll();
    m_lockForOperation.unlock();
}

void ThreadFrameTimer::run()
{
    qDebug() << "ThreadFrameTimer ThreadID" << thread()->currentThreadId();

    QElapsedTimer timer;

    m_lock.lockForWrite();
    m_isStopRequested = false;
    m_lock.unlock();

    do {
        timer.start();

        m_lockForOperation.lockForWrite();

        emit tick(&m_lockForOperation, &m_waitOperation);
//        qDebug() << "ThreadFrameTimer tick";
        m_waitOperation.wait(&m_lockForOperation);

        m_lockForOperation.unlock();

        int waitTime = int(m_msecsPerFrame - double(timer.elapsed()));

//        qDebug() << waitTime;

        if (waitTime > 0)
            thread()->msleep(ulong(waitTime));
    } while (!isStopRequested());

    qDebug() << "ThreadFrameTimer : Finish";
}

bool ThreadFrameTimer::isStopRequested() const
{
    QReadLocker locker(&m_lock);

    return m_isStopRequested;
}
