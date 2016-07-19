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
#ifndef UTILITY_H
#define UTILITY_H

#include <QList>
#include <QPoint>

class Utility
{
public:
    Utility() = default;
    ~Utility() = default;

    static QList<QPoint> getAroundPositions(const QPoint &center)
    {
        int x = center.x();
        int y = center.y();

        QList<QPoint> result = {
            QPoint(x - 1, y - 1),
            QPoint(x - 1, y    ),
            QPoint(x - 1, y + 1),
            QPoint(x    , y - 1),
            QPoint(x    , y + 1),
            QPoint(x + 1, y - 1),
            QPoint(x + 1, y    ),
            QPoint(x + 1, y + 1),
        };

        return result;
    }

    template<class T>
    static QList<QPoint> slideVertical2Dlist(QList<T> &list, int xCount, const QPoint &from, const QPoint &to)
    {
        if (from == to || list.isEmpty())
            return QList<QPoint>();

        Q_ASSERT(from.x() == to.x());

        QList<QPoint> changedPos = {from};

        int dy = to.y() > from.y() ? 1 : -1;
        int currentY = from.y();

        for (; currentY != to.y(); currentY += dy) {
            int currentIndex = currentY * xCount + from.x();

            std::swap(list[currentIndex], list[currentIndex + dy * xCount]);

            changedPos << QPoint(to.x(), currentY + dy);
        }

        return changedPos;
    }

    template<class T>
    static QList<QPoint> slideHorizontal2Dlist(QList<T> &list, int xCount, const QPoint &from, const QPoint &to)
    {
        if (from == to || list.isEmpty())
            return QList<QPoint>();

        Q_ASSERT(from.y() == to.y());

        QList<QPoint> changedPos = {from};

        int dx = to.x() > from.x() ? 1 : -1;
        int currentX = from.x();

        for (; currentX != to.x(); currentX += dx) {
            int currentIndex = from.y() * xCount + currentX;

            std::swap(list[currentIndex], list[currentIndex + dx]);

            changedPos << QPoint(currentX + dx, to.y());
        }

        return changedPos;
    }
};

#endif // UTILITY_H
