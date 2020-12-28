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
#ifndef BOARDINFORMATION_H
#define BOARDINFORMATION_H

#include <QRectF>
#include <QSharedPointer>

class BoardInformation
{
public:
    BoardInformation(const QSize &countXY, const QSize &pixelSize);

    QRectF rectFromPiecePos(const QPoint &piecePos) const;
    QPoint piecePosFromPixelPos(const QPoint &pixelPos) const;

    int countX() const;
    int countY() const;
    QSize countXY() const;
    int pieceCount() const;

    const QSize &boardPixelSize() const;

private:
    QSize m_countXY;
    QSize m_pixelSize;
};

typedef QSharedPointer<BoardInformation> BoardInfoPointer;

#endif // BOARDINFORMATION_H
