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
#ifndef FIFTEENSLIDESHUFFLER_H
#define FIFTEENSLIDESHUFFLER_H

#include "FifteenAbstractShuffler.h"
#include "IPuzzlePiece.h"

namespace Fifteen {

class SlideShuffler : public AbstractShuffler
{
public:
    SlideShuffler(QList<FifteenPiecePointer> &pieces, BoardInfoPointer boardInfo, QPoint &blankPos);

    void exec() override;

private:
    enum Direction : char { // Don't change each values.
        left = -2,
        top = -1,
        right = 2,
        bottom = 1
    };

    bool isHorizontal(Direction dir) const;
    Direction nextDirection(Direction from) const;
    QPoint nextBlankPosition(Direction to) const;

    QPoint &m_blankPos;
};

} // Fifteen

#endif // FIFTEENSLIDESHUFFLER_H
