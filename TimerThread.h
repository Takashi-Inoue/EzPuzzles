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

#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

#include <QThread>
#include <QMutex>
#include <QReadWriteLock>
#include <QWaitCondition>

class TimerThread : public QThread
{
    Q_OBJECT
public:
    explicit TimerThread(QObject *parent = 0);
    ~TimerThread() = default;

signals:
    void tick(QMutex *);

public slots:
    void stop();
    void setFPS(double fps);
    void onOperationFinished();

protected:
    void run();

private:
    bool isStopped();

    QReadWriteLock rwlock;
    bool stopFlag;

    QMutex operationMutex;
    QWaitCondition waitFinishOperation;

    double msecPerFrame;
};

#endif // TIMERTHREAD_H
