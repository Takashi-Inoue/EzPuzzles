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
#ifndef SLIDESHUFFLER_H
#define SLIDESHUFFLER_H

#include "IShuffler.h"
#include <QPoint>
#include <QSize>
#include <random>

class SlideShuffler : public IShuffler
{
    Q_OBJECT
public:
    SlideShuffler(QList<int> &pieces, const QSize &xy, const QPoint &blankPos, QObject *parent = 0);
    ~SlideShuffler() = default;

    void exec() override;

private:
    enum Direction {
        left = -2,
        top = -1,
        right = 2,
        bottom = 1
    };

    Direction nextDirection(Direction from) const;
    Direction reverse(Direction dir) const;
    bool isHorizontal(Direction dir) const;
    QPoint nextBlankPosition(Direction to) const;

    QList<int> &pieces;
    QSize xy;
    QPoint blankPos;

    QList<int> changedIndex;

    mutable std::mt19937 mt;
};

#endif // SLIDESHUFFLER_H
