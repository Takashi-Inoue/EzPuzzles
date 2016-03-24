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
#include "SlideShuffler.h"
#include "PieceMover.h"
#include <QDebug>

SlideShuffler::SlideShuffler(QList<int> &pieces, const QSize &xy, const QPoint &blankPos, QObject *parent) :
    IShuffler(parent),
    pieces(pieces),
    xy(xy),
    blankPos(blankPos),
    mt(std::random_device()())
{
}

void SlideShuffler::exec()
{
    Direction from = right;

    for (int i = 0, lim = xy.width() * xy.height() * 8; i < lim; ++i) {
        Direction to = nextDirection(from);
        QPoint nextPos = nextBlankPosition(to);

        changedIndex = isHorizontal(to) ? PieceMover(pieces, xy).slideHorizontal(blankPos, nextPos)
                                        : PieceMover(pieces, xy).slideVertical(blankPos, nextPos);

        blankPos = nextPos;
        from = reverse(to);

        emit update(changedIndex);
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

        if (blankPos.x() < xy.width() - 1)
            dirs << right;

        break;

    case left:
    case right:
        if (blankPos.y() > 0)
            dirs << top;

        if (blankPos.y() < xy.height() - 1)
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
        next.rx() += (mt() % (xy.width() - blankPos.x() - 1) + 1);
        return next;
    }

    if (to == top) {
        next.ry() = mt() % blankPos.y();
        return next;
    }

    next.ry() += (mt() % (xy.height() - blankPos.y() - 1) + 1);
    return next;
}
