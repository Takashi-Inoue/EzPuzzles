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
#include "Utility.h"

#include <QRandomGenerator>

namespace Fifteen {

SlideShuffler::SlideShuffler(QList<FifteenPiecePointer> &pieces, BoardInfoPointer boardInfo
                           , QPoint &blankPos)
    : AbstractShuffler(pieces, boardInfo)
    , m_blankPos(blankPos)
{
}

void SlideShuffler::exec()
{
    Q_ASSERT(!m_pieces.isEmpty());
    Q_ASSERT(m_boardInfo != nullptr);

    Direction from = Direction::right;

    for (int i = 0, lim = m_boardInfo->pieceCount() * 4; i < lim; ++i) {
        Direction to = nextDirection(from);
        QPoint nextBlankPos = nextBlankPosition(to);

        auto slider = isHorizontal(to) ? &Utility::slideHorizontal2Dlist<FifteenPiecePointer>
                                       : &Utility::slideVertical2Dlist<FifteenPiecePointer>;

        QList<QPoint> changedPos = slider(m_pieces, m_boardInfo->xCount(), m_blankPos, nextBlankPos);

        m_blankPos = nextBlankPos;
        from = Direction(-to);

        for (const QPoint &pos : changedPos) {
            FifteenPiecePointer &piece = m_pieces[pos.y() * m_boardInfo->xCount() + pos.x()];

            piece->setPosWithoutAnimation(pos);
        }
    }
}

bool SlideShuffler::isHorizontal(Direction dir) const
{
    return char(dir) % 2 == 0;
}

SlideShuffler::Direction SlideShuffler::nextDirection(Direction from) const
{
    auto randomDirection = [](int pos, int max) -> char
    {
        if (pos == 0)
            return 1;
        else if (pos == max)
            return -1;

        return (QRandomGenerator::global()->bounded(2) == 0) ? 1 : -1;
    };

    return isHorizontal(from) ? Direction(randomDirection(m_blankPos.y(), m_boardInfo->yCount() - 1))
                              : Direction(randomDirection(m_blankPos.x(), m_boardInfo->xCount() - 1) * 2);
}

QPoint SlideShuffler::nextBlankPosition(Direction to) const
{
    QRandomGenerator *randomGenerator = QRandomGenerator::global();
    QPoint next(m_blankPos);

    switch (to) {
    case Direction::left:
        next.rx() = randomGenerator->bounded(m_blankPos.x());
        return next;

    case Direction::right:
        next.rx() = randomGenerator->bounded(m_blankPos.x() + 1, m_boardInfo->xCount());
        return next;

    case Direction::top:
        next.ry() = randomGenerator->bounded(m_blankPos.y());
        return next;

    case Direction::bottom:
        next.ry() = randomGenerator->bounded(m_blankPos.y() + 1, m_boardInfo->yCount());
        return next;
    }
}

} // Fifteen
