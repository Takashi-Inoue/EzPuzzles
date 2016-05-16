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
#include "PuzzlePiece.h"
#include "ImageFragmentPiece.h"
#include "SourceImage.h"

namespace Fifteen {

PuzzlePiece::PuzzlePiece(const QPoint &pos, const QPixmap &sourceImage, const QRectF &sourceRect) :
    imagePiece(std::make_unique<ImageFragmentPiece>(sourceImage, sourceRect)),
    position(pos),
    defaultPosition(pos)
{
}

void PuzzlePiece::draw(QPainter &painter, const QPointF &pos)
{
    imagePiece->draw(painter, pos);
}

void PuzzlePiece::draw(QPainter &painter, const QPointF &pos, const QSizeF &targetSize)
{
    imagePiece->draw(painter, pos, targetSize);
}

void PuzzlePiece::setPos(const QPoint &position)
{
    this->position = position;
}

void PuzzlePiece::swapPos(IPuzzlePiece *other)
{
    QPoint otherPos = other->currentPos();

    other->setPos(position);

    position = otherPos;
}

QPoint PuzzlePiece::currentPos() const
{
    return position;
}

QPoint PuzzlePiece::defaultPos() const
{
    return defaultPosition;
}

bool PuzzlePiece::isPosCorrect() const
{
    return position == defaultPosition;
}

} // Fifteen
