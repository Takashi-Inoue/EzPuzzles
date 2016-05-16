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

SwapShuffler::SwapShuffler(QList<QList<PuzzlePiecePointer>> &pieces) :
    pieces(pieces),
    mt(std::random_device()())
{
}

void SwapShuffler::shufflePieces()
{
    Q_ASSERT(!pieces.isEmpty());

    int width  = pieces.front().size();
    int height = pieces.size();

    QList<QPoint> changedPos;

    for (int i = width * height - 1; i > 0; --i) {
        changedPos.clear();

        int r = mt() % i;

        int ly = i / width;
        int lx = i % width;
        int ry = r / width;
        int rx = r % width;

        auto &lhs = pieces[ly][lx];
        auto &rhs = pieces[ry][rx];

        lhs->swapPos(rhs.get());
        lhs.swap(rhs);

        changedPos << QPoint(lx, ly) << QPoint(rx, ry);

        emit update(changedPos);
    }
}

} // Fifteen
