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
#ifndef FRAMETIMER_H
#define FRAMETIMER_H

#include "ThreadOperation.h"

class FrameTimer : public ThreadOperation
{
    Q_OBJECT
public:
    FrameTimer(double fps = 60.0);
    ~FrameTimer() = default;

    void stop() override;

signals:
    void tick(QMutex *mutexOperation, QWaitCondition *waitOperation);

protected:
    QString className() const override;
    void execImpl() override;

private:
    QMutex mutexOperation;
    QWaitCondition waitOperation;

    double msecsPerFrame;
};

#endif // FRAMETIMER_H
