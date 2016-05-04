/*
 * Copyright YEAR Takashi Inoue
 *
 * This file is part of APPNAME.
 *
 * APPNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * APPNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with APPNAME.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef FIFTEENSLIDESHUFFLER_H
#define FIFTEENSLIDESHUFFLER_H

#include "FifteenIShuffler.h"
#include "IPuzzlePiece.h"

#include <random>

namespace Fifteen {

class SlideShuffler : public IShuffler
{
public:
    SlideShuffler(QList<QList<PuzzlePiecePointer>> &pieces, QPoint &blankPos);
    ~SlideShuffler() = default;

    void shufflePieces() override;

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

    QList<QList<PuzzlePiecePointer>> &pieces;
    QPoint &blankPos;
    int width;
    int height;

    mutable std::mt19937 mt;
};

} // Fifteen

#endif // FIFTEENSLIDESHUFFLER_H
