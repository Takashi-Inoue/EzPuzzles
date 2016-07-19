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
#include "FifteenSwapShuffler.h"
#include "FifteenPieceMover.h"

namespace Fifteen {

SwapShuffler::SwapShuffler(QList<PuzzlePiecePointer> &pieces, const BoardInfoPointer &boardInfo) :
    pieces(pieces),
    boardInfo(boardInfo),
    mt(std::random_device()())
{
}

void SwapShuffler::shufflePieces()
{
    Q_ASSERT(!pieces.isEmpty());
    Q_ASSERT(boardInfo != nullptr);

    QList<QPoint> changedPos;

    int xCount = boardInfo->xCount();

    for (int i = pieces.size() - 1; i > 0; --i) {
        changedPos.clear();

        int r = mt() % i;

        auto &lhs = pieces[i];
        auto &rhs = pieces[r];

        int lx = i % xCount;
        int ly = i / xCount;
        int rx = r % xCount;
        int ry = r / xCount;

        lhs.swap(rhs);
        lhs->setPosWithoutAnimation(QPoint(lx, ly));
        rhs->setPosWithoutAnimation(QPoint(rx, ry));

        changedPos << QPoint(lx, ly) << QPoint(rx, ry);

        emit update(changedPos);
    }
}

} // Fifteen
