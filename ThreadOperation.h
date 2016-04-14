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

#ifndef THREADOPERATION_H
#define THREADOPERATION_H

#include <QMutexLocker>
#include <QObject>
#include <QWaitCondition>

class ThreadOperation : public QObject
{
    Q_OBJECT
public:
    explicit ThreadOperation(QObject *parent = 0);
    virtual ~ThreadOperation();

public slots:
    void start();
    void stop();
    void wait();
    void suspend();
    void resume();
    bool isRunning();

signals:
    void startOperation();
    void started();
    void aborted();
    void completed();

protected slots:

protected:
    virtual QString className() const;
    virtual void execImpl() = 0;
    bool isStopped() const;

    QWaitCondition waitSuspend;
    mutable QMutex mutexSuspend;
    bool suspended;

private slots:
    void exec();

private:
    mutable QMutex mutex;
    QMutex mutexRunning;
    bool stopped;

    ThreadOperation(const ThreadOperation &) = delete;
    ThreadOperation(ThreadOperation &&) = delete;
    ThreadOperation &operator=(const ThreadOperation &) = delete;
    ThreadOperation &operator=(ThreadOperation &&) = delete;
};

#endif // THREADOPERATION_H
