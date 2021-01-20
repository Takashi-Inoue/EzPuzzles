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

#include "FifteenFactory.h"

#include "AnimationObject/Effect/EffectGraduallyBlinkFrame.h"
#include "PuzzlePiece.h"

namespace Fifteen {

FifteenPiecePointer Factory::createPiece(BoardInfoPointer boardInfo, const QPoint &defaultPos
                                       , const QPixmap &sourceImage) const
{
    return QSharedPointer<Fifteen::PuzzlePiece>::create(boardInfo, defaultPos, sourceImage);
}

QList<FifteenPiecePointer> Factory::createPieces(BoardInfoPointer boardInfo
                                               , const QPixmap &sourceImage) const
{
    QList<FifteenPiecePointer> pieces;

    pieces.reserve(boardInfo->pieceCount());

    int xCount = boardInfo->xCount();

    for (int i = 0, lim = xCount * boardInfo->yCount(); i < lim; ++i)
        pieces << createPiece(boardInfo, QPoint(i % xCount, i / xCount), sourceImage);

    return pieces;
}

QList<FifteenPiecePointer> Factory::createPieces(BoardInfoPointer boardInfo
                                               , const QPixmap &sourceImage
                                               , const QList<QPoint> &defaultPositions) const
{
    if (defaultPositions.isEmpty())
        return createPieces(boardInfo, sourceImage);

    QList<FifteenPiecePointer> pieces;

    pieces.reserve(boardInfo->pieceCount());

    int xCount = boardInfo->xCount();

    for (int i = 0, lim = boardInfo->pieceCount(); i < lim; ++i) {
        FifteenPiecePointer piece = createPiece(boardInfo, defaultPositions.at(i), sourceImage);

        piece->setPosWithoutAnimation(QPoint(i % xCount, i / xCount));
        pieces << piece;
    }

    return pieces;
}


} // namespace Fifteen
