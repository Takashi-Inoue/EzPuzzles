/*
 * Copyright YEAR Takashi Inoue
 *
 * This file is part of APPNAME.
 *
 * APPNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * APPNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with APPNAME.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef IPHASE_H
#define IPHASE_H

#include <QPainter>
#include <QString>

#include <memory>

class IPhase : public QObject
{
    Q_OBJECT
public:
    enum PhaseType {
        PhaseReady,
        PhasePreGame,
        PhaseGaming,
        PhaseEnding,
        PhaseCleared,
    };

    explicit IPhase(QObject *parent = nullptr) :
        QObject(parent)
    {
    }

    virtual ~IPhase() = default;

    virtual void click(const QPoint &) = 0;
    virtual void onTickFrame() = 0;
    virtual void draw(QPainter &) = 0;
    virtual bool canSave() const = 0;
    virtual bool canLoad() const = 0;
    virtual bool save(QDataStream &stream) const = 0;
    virtual bool load(QDataStream &stream) = 0;
    virtual QString information() const = 0;

signals:
    void toNextPhase(PhaseType);
};

typedef std::shared_ptr<IPhase> PhasePointer;

#endif // IPHASE_H
