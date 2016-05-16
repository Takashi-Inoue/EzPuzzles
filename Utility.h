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
};

#endif // UTILITY_H
