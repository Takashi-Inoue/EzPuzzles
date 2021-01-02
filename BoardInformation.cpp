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

BoardInformation::BoardInformation(const QSize &xyCount, const QSize &pixelSize) :
    m_xyCount(xyCount),
    m_pixelSize(pixelSize)
{
    Q_ASSERT(xyCount.isValid());
    Q_ASSERT(!pixelSize.isEmpty());
}

QRectF BoardInformation::rectFromPiecePos(const QPoint &piecePos) const
{
    double x = piecePos.x();
    double y = piecePos.y();

    QPointF tl((x       * m_pixelSize.width()) / xCount(), (y       * m_pixelSize.height()) / yCount());
    QPointF br(((x + 1) * m_pixelSize.width()) / xCount(), ((y + 1) * m_pixelSize.height()) / yCount());

    return QRectF(tl, br);
}

QPoint BoardInformation::piecePosFromPixelPos(const QPoint &pixelPos) const
{
    return QPoint((pixelPos.x() * int(xCount())) / m_pixelSize.width(),
                  (pixelPos.y() * int(yCount())) / m_pixelSize.height());
}

int BoardInformation::xCount() const
{
    return m_xyCount.width();
}

int BoardInformation::yCount() const
{
    return m_xyCount.height();
}

QSize BoardInformation::xyCount() const
{
    return m_xyCount;
}

int BoardInformation::pieceCount() const
{
    return xCount() * yCount();
}

const QSize &BoardInformation::boardPixelSize() const
{
    return m_pixelSize;
}
