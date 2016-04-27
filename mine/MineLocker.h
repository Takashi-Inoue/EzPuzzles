﻿/*
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
#ifndef MINELOCKER_H
#define MINELOCKER_H

#include "IMinePiece.h"
#include <QVector>

namespace MineSweeper {

class MineLocker
{
public:
    MineLocker(QVector<QVector<MinePiecePointer>> &pieces);
    ~MineLocker() = default;

    void addMinePos(const QPoint &minePos);
    void addMinesPositions(const QList<QPoint> &minesPositions);
    QList<QPoint> lockMines();

private:
    QList<QPoint> checkMinesForLock(const QPoint &minePos);
    QList<QPoint> checkAroundMinesForLock(const QPoint &openedNearMinePos);

    MinePiecePointer &getPiece(const QPoint &pos);
    const MinePiecePointer &getPiece(const QPoint &pos) const;
    QList<QPoint> getAroundOpenedPositions(const QPoint &pos) const;

    QVector<QVector<MinePiecePointer>> &pieces;
    QList<QPoint> notLockedMinesPos;

    MineLocker(const MineLocker &) = delete;
    MineLocker(MineLocker &&) = delete;
    MineLocker &operator=(const MineLocker &) = delete;
    MineLocker &operator=(MineLocker &&) = delete;
};

} // MineSweeper

#endif // MINELOCKER_H
