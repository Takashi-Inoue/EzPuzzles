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

void SafePiece::draw(QPainter &painter, const QPoint &pos)
{
    if (isOpen())
        fillRect(painter, pos, size);

    switchImagePiece->draw(painter, pos);

    if (numberPiece != nullptr && isOpen())
        numberPiece->draw(painter, pos);
}

void SafePiece::draw(QPainter &painter, const QPoint &pos, const QSize &targetSize)
{
    if (isOpen())
        fillRect(painter, pos, targetSize);

    switchImagePiece->draw(painter, pos, targetSize);

    if (numberPiece != nullptr && isOpen())
        numberPiece->draw(painter, pos, targetSize);
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

void SafePiece::fillRect(QPainter &painter, const QPoint &pos, const QSize &targetSize)
{
    painter.setOpacity(1);

    isNearMine() ? painter.fillRect(QRect(pos, targetSize), Qt::white)
                 : painter.fillRect(QRect(pos, targetSize), Qt::black);
}

} // MineSweeper
