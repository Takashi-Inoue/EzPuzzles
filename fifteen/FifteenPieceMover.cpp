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
#include "Utility.h"

namespace Fifteen {

PieceMover::PieceMover(QList<PuzzlePiecePointer> &pieces, int xCount) :
    pieces(pieces),
    xCount(xCount)
{
}

QList<PuzzlePiecePointer> PieceMover::slideVertical(const QPoint &from, const QPoint &to)
{
    if (from == to || pieces.isEmpty())
        return QList<PuzzlePiecePointer>();

    Q_ASSERT(from.x() == to.x());

    auto changedPos = Utility::slideVertical2Dlist<PuzzlePiecePointer>(pieces, xCount, from, to);

    QList<PuzzlePiecePointer> changedPieces;

    for (const auto &pos : changedPos) {
        auto piece = pieces[pos.y() * xCount + pos.x()];

        piece->setPos(pos);

        changedPieces << piece;
    }

    return changedPieces;
}

QList<PuzzlePiecePointer> PieceMover::slideHorizontal(const QPoint &from, const QPoint &to)
{
    if (from == to || pieces.isEmpty())
        return QList<PuzzlePiecePointer>();

    Q_ASSERT(from.y() == to.y());

    auto changedPos = Utility::slideHorizontal2Dlist<PuzzlePiecePointer>(pieces, xCount, from, to);

    QList<PuzzlePiecePointer> changedPieces;

    for (const auto &pos : changedPos) {
        auto piece = pieces[pos.y() * xCount + pos.x()];

        piece->setPos(pos);

        changedPieces << piece;
    }

    return changedPieces;
}

} // Fifteen
