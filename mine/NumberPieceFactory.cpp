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
#include "NumberPieceFactory.h"
#include "NumberPiece.h"

#include <QColor>

namespace MineSweeper {

const QList<QColor> NumColor = {
    QColor(  0,   0, 255),
    QColor(  0, 160,   0),
    QColor(255,   0,   0),
    QColor(  0,   0, 128),
    QColor(128,   0,   0),
    QColor(  0, 128, 128),
    QColor( 64,  64,   0),
    QColor(  0,   0,   0),
};

QMap<NumberPieceFactory::keyPair, NumberPieceFactory::PieceList> NumberPieceFactory::piecesMap;

IPiece *NumberPieceFactory::getPiece(int number, const QSize &size)
{
    Q_ASSERT(number >= 0 && number <= NumColor.size());

    auto &pieceList = piecesMap[keyPair(size.width(), size.height())];

    if (pieceList.isEmpty())
        pieceList.resize(NumColor.size());

    auto &piece = pieceList[number - 1];

    if (piece == nullptr)
        piece = std::make_shared<NumberPiece>(number, NumColor.at(number - 1), size);

    return piece.get();
}

} // MineSweeper
