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
#include "BoardInformation.h"

BoardInformation::BoardInformation(const QSize &countXY, const QSize &pixelSize) :
    m_countXY(countXY),
    m_pixelSize(pixelSize)
{
    Q_ASSERT(countXY.isValid());
    Q_ASSERT(!pixelSize.isEmpty());
}

QRectF BoardInformation::rectFromPiecePos(const QPoint &piecePos) const
{
    double x = piecePos.x();
    double y = piecePos.y();

    QPointF tl((x       * m_pixelSize.width()) / countX(), (y       * m_pixelSize.height()) / countY());
    QPointF br(((x + 1) * m_pixelSize.width()) / countX(), ((y + 1) * m_pixelSize.height()) / countY());

    return QRectF(tl, br);
}

QPoint BoardInformation::piecePosFromPixelPos(const QPoint &pixelPos) const
{
    return QPoint((pixelPos.x() * int(countX())) / m_pixelSize.width(),
                  (pixelPos.y() * int(countY())) / m_pixelSize.height());
}

int BoardInformation::countX() const
{
    return m_countXY.width();
}

int BoardInformation::countY() const
{
    return m_countXY.height();
}

QSize BoardInformation::countXY() const
{
    return m_countXY;
}

int BoardInformation::pieceCount() const
{
    return countX() * countY();
}

const QSize &BoardInformation::boardPixelSize() const
{
    return m_pixelSize;
}
