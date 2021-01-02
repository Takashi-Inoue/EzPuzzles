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
#ifndef MINEFIELD_H
#define MINEFIELD_H

#include "MinePiece.h"

#include <QList>
#include <QSharedPointer>

namespace MineSweeper {

class MineField
{
public:
    using MinePiece2DList = QList<QList<MinePiecePointer>>;

    MineField(MinePiece2DList &pieces, bool isAutoLock, int mineCount);
    MineField(MinePiece2DList &pieces, bool isAutoLock, int mineCount
            , int openedCount, int missedCount);

    void open(const QPoint &pos);

    int openedCount() const;
    int missedCount() const;
    double openedRate() const;
    double mineRatio() const;
    bool isAllOpened() const;
    bool isNoMissed() const;

    QString information() const;
    const QList<QPoint> &explodedPositions() const;

private:
    void lockMines(QList<QPoint> &pointsToCheckToLock);
    void lockMinesInAround(int x, int y);
    void openChaining(const QPoint &pos, QList<QPoint> &openedPointsNearMines);

    int totalPieceCount() const;
    int safePiecesCount() const;

    QList<QPoint> aroundPos(const QPoint &centerPos) const;
    void gatherAroundPointsToLock(const QPoint &centerPos, QList<QPoint> &openedPointsNearMines) const;

    MinePiece2DList &pieces;
    int m_mineCount;
    int m_openedCount;
    int m_missedCount;

    bool m_isAutoLock;

    QList<QPoint> explodedPos;
};

using MineFieldPointer = QSharedPointer<MineField>;

} // MineSweeper

#endif // MINEFIELD_H
