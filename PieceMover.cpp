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
#include "PieceMover.h"

PieceMover::PieceMover(QList<int> &pieces, QSize xy) :
    pieces(pieces),
    xy(xy)
{
}

QList<int> PieceMover::slideVertical(const QPoint &oldPos, const QPoint &newPos)
{
    QList<int> changedIndex;

    int x = newPos.x();
    int y = oldPos.y();
    int dy = newPos.y() < oldPos.y() ? -1 : 1;

    for (; y != newPos.y(); y += dy) {
        std::swap(pieces[y * xy.width() + x], pieces[(y + dy) * xy.width() + x]);

        changedIndex << y * xy.width() + x;
    }

    changedIndex << y * xy.width() + x;

    return changedIndex;
}

QList<int> PieceMover::slideHorizontal(const QPoint &oldPos, const QPoint &newPos)
{
    QList<int> changedIndex;

    int x = oldPos.x();
    int y = newPos.y() * xy.width();
    int dx = newPos.x() < oldPos.x() ? -1 : 1;

    for (; x != newPos.x(); x += dx) {
        std::swap(pieces[y + x], pieces[y + x + dx]);

        changedIndex << y + x;
    }

    changedIndex << y + x;

    return changedIndex;
}

void PieceMover::swap(const QPoint &p1, const QPoint &p2)
{
    int idx1 = p1.y() * xy.width() + p1.x();
    int idx2 = p2.y() * xy.width() + p2.x();

    std::swap(pieces[idx1], pieces[idx2]);
}
