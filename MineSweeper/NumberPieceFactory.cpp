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
#include "NumberPieceFactory.h"
#include "NumberPiece.h"

#include <QColor>

namespace MineSweeper {

QHash<QSize, NumberPieceFactory::PieceList> NumberPieceFactory::m_piecesHash;

QSharedPointer<IPiece> NumberPieceFactory::getPiece(int number, const QSize &size)
{
    static const QList<QColor> numberColors = {
        QColor(  0,   0, 255),
        QColor(  0, 160,   0),
        QColor(255,   0,   0),
        QColor(  0,   0, 128),
        QColor(128,   0,   0),
        QColor(  0, 128, 128),
        QColor( 64,  64,   0),
        QColor(  0,   0,   0),
    };

    Q_ASSERT(number >= 1 && number <= numberColors.size());

    PieceList &pieceList = m_piecesHash[size];

    if (pieceList.isEmpty())
        pieceList.resize(numberColors.size());

    PiecePointer &piece = pieceList[number - 1];

    if (piece == nullptr)
        piece = QSharedPointer<NumberPiece>::create(number, numberColors[number - 1], size);

    return piece;
}

} // MineSweeper
