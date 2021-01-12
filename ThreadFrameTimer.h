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

#ifndef THREADFRAMETIMER_H
#define THREADFRAMETIMER_H

#include <QThread>
#include <QReadWriteLock>
#include <QWaitCondition>

class ThreadFrameTimer : public QThread
{
    Q_OBJECT
public:
    ThreadFrameTimer(ushort fps = 60, QObject *parent = nullptr);

    void stop();

signals:
    void tick(QReadWriteLock *lock, QWaitCondition *wait);

protected:
    void run() override;

private:
    bool isStopRequested() const;

    mutable QReadWriteLock m_lock;

    QReadWriteLock m_lockForOperation;
    QWaitCondition m_waitOperation;

    bool m_isStopRequested = false;
    double m_msecsPerFrame = 0.0;
};

#endif // THREADFRAMETIMER_H
