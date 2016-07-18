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
#include "SafePiece.h"

#include "SwitchImagePiece.h"
#include "NumberPieceFactory.h"

namespace MineSweeper {

SafePiece::SafePiece(int numOfAroundMines, const QPixmap &pixmap, const QRect &sourceRect) :
    switchImagePiece(std::make_unique<SwitchImagePiece>(pixmap, sourceRect)),
    numberPiece(nullptr),
    numOfAroundMines(numOfAroundMines),
    size(pixmap.size())
{
    Q_ASSERT(numOfAroundMines >= 0 && numOfAroundMines < 9);

    if (!sourceRect.isNull())
        size = sourceRect.size();

    if (isNearMine())
        switchImagePiece->setOpenPieceOpacity(0.5);

    if (numOfAroundMines > 0)
        numberPiece = NumberPieceFactory::getPiece(numOfAroundMines, size);
}

void SafePiece::draw(QPainter &painter, const QPointF &pos)
{
    if (isOpen())
        fillRect(painter, QRectF(pos, size));

    switchImagePiece->draw(painter, pos);

    if (numberPiece != nullptr && isOpen())
        numberPiece->draw(painter, pos);
}

void SafePiece::draw(QPainter &painter, const QRectF &rect)
{
    if (isOpen())
        fillRect(painter, rect);

    switchImagePiece->draw(painter, rect);

    if (numberPiece != nullptr && isOpen())
        numberPiece->draw(painter, rect);
}

void SafePiece::open()
{
    switchImagePiece->open();
}

void SafePiece::close()
{
    switchImagePiece->close();
}

void SafePiece::lock()
{
    switchImagePiece->lock();
}

bool SafePiece::isOpen() const
{
    return switchImagePiece->isOpen();
}

bool SafePiece::isLock() const
{
    return switchImagePiece->isLock();
}

void SafePiece::setOpenPieceOpacity(double opacity)
{
    if (!isNearMine())
        switchImagePiece->setOpenPieceOpacity(opacity);
}

bool SafePiece::isMine() const
{
    return false;
}

bool SafePiece::isNearMine() const
{
    return numOfAroundMines > 0;
}

bool SafePiece::isWall() const
{
    return false;
}

int SafePiece::numberOfAroundMines() const
{
    return numOfAroundMines;
}

void SafePiece::fillRect(QPainter &painter, const QRectF &rect)
{
    painter.setOpacity(1);

    isNearMine() ? painter.fillRect(rect, Qt::white)
                 : painter.fillRect(rect, Qt::black);
}

} // MineSweeper
