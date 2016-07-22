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
#include "FifteenSlideShuffler.h"
#include "FifteenPieceMover.h"
#include "Utility.h"

namespace Fifteen {

SlideShuffler::SlideShuffler(QList<PuzzlePiecePointer> &pieces, const BoardInfoPointer &boardInfo, QPoint &blankPos) :
    AbstractShuffler(pieces, boardInfo),
    blankPos(blankPos),
    mt(std::random_device()())
{
}

QString SlideShuffler::className() const
{
    return "SlideShuffler";
}

void SlideShuffler::execImpl()
{
    Q_ASSERT(!pieces.isEmpty());
    Q_ASSERT(boardInfo != nullptr);

    QList<QPoint> changedPos;

    Direction from = right;

    for (int i = 0, lim = boardInfo->xCount() * boardInfo->yCount() * 4; i < lim; ++i) {
        Direction to = nextDirection(from);
        QPoint nextPos = nextBlankPosition(to);

        changedPos = isHorizontal(to) ? Utility::slideHorizontal2Dlist<PuzzlePiecePointer>(pieces, boardInfo->xCount(), blankPos, nextPos)
                                      : Utility::slideVertical2Dlist<PuzzlePiecePointer>(pieces, boardInfo->xCount(), blankPos, nextPos);
        blankPos = nextPos;
        from = reverse(to);

        QList<PuzzlePiecePointer> changedPieces;

        for (const auto &pos : changedPos) {
            auto &piece = pieces[pos.y() * boardInfo->xCount() + pos.x()];

            piece->setPosWithoutAnimation(pos);

            changedPieces << piece;
        }

        emit update(changedPieces);

        if (isStopped())
            return;
    }
}

SlideShuffler::Direction SlideShuffler::nextDirection(SlideShuffler::Direction from) const
{
    QList<Direction> dirs;

    switch (from) {
    case top:
    case bottom:
        if (blankPos.x() > 0)
            dirs << left;

        if (blankPos.x() < boardInfo->xCount() - 1)
            dirs << right;

        break;

    case left:
    case right:
        if (blankPos.y() > 0)
            dirs << top;

        if (blankPos.y() < boardInfo->yCount() - 1)
            dirs << bottom;

        break;
    }

    if (dirs.size() == 1)
        return dirs.first();

    return dirs.at(mt() % dirs.size());
}

SlideShuffler::Direction SlideShuffler::reverse(SlideShuffler::Direction dir) const
{
    return static_cast<Direction>(-dir);
}

bool SlideShuffler::isHorizontal(SlideShuffler::Direction dir) const
{
    return ((dir == left) | (dir == right));
}

QPoint SlideShuffler::nextBlankPosition(SlideShuffler::Direction to) const
{
    QPoint next(blankPos);

    if (to == left) {
        next.rx() = mt() % blankPos.x();
        return next;
    }

    if (to == right) {
        next.rx() += (mt() % (boardInfo->xCount() - blankPos.x() - 1) + 1);
        return next;
    }

    if (to == top) {
        next.ry() = mt() % blankPos.y();
        return next;
    }

    // bottom
    next.ry() += (mt() % (boardInfo->yCount() - blankPos.y() - 1) + 1);
    return next;
}

} // Fifteen
