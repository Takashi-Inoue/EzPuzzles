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
#ifndef ABSTRACTPHASE_H
#define ABSTRACTPHASE_H

#include <QPainter>
#include <QSharedPointer>
#include <QString>

class AbstractPhase : public QObject
{
    Q_OBJECT
public:
    enum PhaseType : qint8 {
        PhaseReady,
        PhaseGaming,
        PhaseEnding,
        PhaseCleared,
    };

    Q_ENUM(PhaseType)

    AbstractPhase(PhaseType nextPhaseType, QObject *parent = nullptr)
        : QObject(parent)
        , m_nextPhaseType(nextPhaseType)
    {}

    virtual void click(const QPoint &) = 0;
    virtual void press(const QPoint &) {}
    virtual void release(const QPoint &) {}
    virtual void onTickFrame() = 0;
    virtual void draw(QPainter &) = 0;
    virtual bool canSave() const = 0;
    virtual bool canLoad() const = 0;
    virtual QString information() const = 0;

signals:
    void toNextPhase(AbstractPhase::PhaseType);

protected:
    const PhaseType m_nextPhaseType = PhaseReady;
};

using PhasePointer = QSharedPointer<AbstractPhase>;

#endif // ABSTRACTPHASE_H
