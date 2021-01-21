/*
 * Copyright 2021 Takashi Inoue
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

#ifndef FIFTEEN_FACTORY_H
#define FIFTEEN_FACTORY_H

#include "AnimationObject/Effect/AbstractEffect.h"
#include "BoardInformation.h"
#include "IPuzzlePiece.h"

namespace Fifteen {

class Factory
{
public:
    Factory() = default;
    virtual ~Factory() = default;

    virtual FifteenPiecePointer createPiece(BoardInfoPointer boardInfo, const QPoint &defaultPos
                                          , const QPixmap &sourceImage) const;

    QList<FifteenPiecePointer> createPieces(BoardInfoPointer boardInfo
                                          , const QPixmap &sourceImage) const;
    QList<FifteenPiecePointer> createPieces(BoardInfoPointer boardInfo
                                          , const QPixmap &sourceImage
                                          , const QList<QPoint> &defaultPositions) const;
};

} // namespace Fifteen

#endif // FIFTEEN_FACTORY_H
