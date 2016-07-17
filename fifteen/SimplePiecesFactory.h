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
#ifndef SIMPLEPIECESFACTORY_H
#define SIMPLEPIECESFACTORY_H

#include "IPuzzlePiece.h"
#include <QPixmap>

class QDataStream;

namespace Fifteen {

class SimplePiecesFactory
{
public:
    static PuzzlePiecePointer createPiece(const QPoint &posInArray, const QPixmap &pixmap);

    SimplePiecesFactory(const QPixmap &sourcePixmap, const QSize &xy);
    ~SimplePiecesFactory() = default;

    QList<QList<PuzzlePiecePointer>> createPieces() const;
    QList<QList<PuzzlePiecePointer>> createPieces(const QList<QPoint> &defaultPositions) const;

private:
    PuzzlePiecePointer createPiece(const QPoint &defaultPos) const;

    QPixmap sourcePixmap;

    QList<double> splitterXs;
    QList<double> splitterYs;
};

} // Fifteen

#endif // SIMPLEPIECESFACTORY_H