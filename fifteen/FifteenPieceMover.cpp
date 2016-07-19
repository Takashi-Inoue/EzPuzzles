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
#include "FifteenPieceMover.h"

namespace Fifteen {

PieceMover::PieceMover(QList<PuzzlePiecePointer> &pieces, int xCount) :
    pieces(pieces),
    xCount(xCount)
{
}

QList<QPoint> PieceMover::slideVertical(const QPoint &from, const QPoint &to)
{
    if (from == to || pieces.isEmpty())
        return QList<QPoint>();

    Q_ASSERT(from.x() == to.x());

    QList<QPoint> changedPos = {from};

    int dy = to.y() > from.y() ? 1 : -1;
    int currentY = from.y();

    for (; currentY != to.y(); currentY += dy) {
        int currentIndex = currentY * xCount + from.x();
        int nextIndex = currentIndex + dy * xCount;

        auto &currentPiece = pieces[currentIndex];
        auto &nextPiece    = pieces[nextIndex];

        currentPiece.swap(nextPiece);

        changedPos << QPoint(to.x(), currentY + dy);
    }

    for (auto &pos : changedPos)
        pieces[pos.y() * xCount + pos.x()]->setPos(pos);

    return changedPos;
}

QList<QPoint> PieceMover::slideHorizontal(const QPoint &from, const QPoint &to)
{
    if (from == to || pieces.isEmpty())
        return QList<QPoint>();

    Q_ASSERT(from.y() == to.y());

    QList<QPoint> changedPos = {from};

    int dx = to.x() > from.x() ? 1 : -1;
    int currentX = from.x();

    for (; currentX != to.x(); currentX += dx) {
        int currentIndex = from.y() * xCount + currentX;

        auto &currentPiece = pieces[currentIndex];
        auto &nextPiece    = pieces[currentIndex + dx];

        currentPiece.swap(nextPiece);

        changedPos << QPoint(currentX + dx, to.y());
    }

    for (auto &pos : changedPos)
        pieces[pos.y() * xCount + pos.x()]->setPos(pos);

    return changedPos;
}

} // Fifteen
