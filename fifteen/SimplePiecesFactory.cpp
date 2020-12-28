﻿/*
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
#include "SimplePiecesFactory.h"
#include "PuzzlePiece.h"

namespace Fifteen {

SimplePiecesFactory::SimplePiecesFactory(BoardInfoPointer boardInfo, const QPixmap &sourcePixmap) :
    boardInfo(boardInfo),
    sourcePixmap(sourcePixmap)
{
    Q_ASSERT(!sourcePixmap.isNull());
}

QList<PuzzlePiecePointer> SimplePiecesFactory::createPieces() const
{
    QList<PuzzlePiecePointer> pieces;

    int countX = boardInfo->countX();

    for (int i = 0, lim = countX * boardInfo->countY(); i < lim; ++i)
        pieces << createPiece(QPoint(i % countX, i / countX));

    return pieces;
}

QList<PuzzlePiecePointer> SimplePiecesFactory::createPieces(const QList<QPoint> &defaultPositions) const
{
    QList<PuzzlePiecePointer> pieces;

    for (int i = 0, lim = boardInfo->countX() * boardInfo->countY(); i < lim; ++i) {
        auto piece = createPiece(defaultPositions.at(i));

        piece->setPosWithoutAnimation(QPoint(i % boardInfo->countX(), i / boardInfo->countX()));

        pieces << piece;
    }

    return pieces;
}

PuzzlePiecePointer SimplePiecesFactory::createPiece(const QPoint &defaultPos) const
{
    return std::make_shared<PuzzlePiece>(boardInfo, defaultPos, sourcePixmap);
}

} // Fifteen
