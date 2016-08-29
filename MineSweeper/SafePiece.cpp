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

SafePiece::SafePiece(int numOfAroundMines, const QRect &destRect, const QPixmap &pixmap, const QRect &sourceRect) :
    switchImagePiece(std::make_unique<SwitchImagePiece>(pixmap, sourceRect)),
    numberPiece(nullptr),
    numOfAroundMines(numOfAroundMines),
    rect(destRect),
    openOpacity(1),
    oldOpacity(1),
    isChanged(true)
{
    Q_ASSERT(numOfAroundMines >= 0 && numOfAroundMines < 9);

    if (isNearMine()) {
        openOpacity = 0.5;
        numberPiece = NumberPieceFactory::getPiece(numOfAroundMines, rect.size());
    }
}

void SafePiece::draw(QPainter &painter)
{
    if (!isChanged)
        return;

    painter.save();

    if (isOpen()) {
        fillRect(painter);
        painter.setOpacity(openOpacity);
    }

    switchImagePiece->draw(painter, rect);

    painter.setOpacity(1);

    if (numberPiece != nullptr && isOpen())
        numberPiece->draw(painter, rect);

    if (effect != nullptr)
        effect->draw(painter, rect);

    painter.restore();

    isChanged = false;
}

void SafePiece::setOpenPieceOpacity(double opacity)
{
    if (isNearMine())
        return;

    openOpacity = opacity;

    double div = opacity - oldOpacity;

    div = (div >= 0) ? div : -div;

    if ((div > 0.01) | (opacity == 1)) {
        oldOpacity = opacity;
        isChanged = true;
    }
}

void SafePiece::setEffect(EffectPointer effect)
{
    this->effect = effect;
}

void SafePiece::onTickFrame()
{
    if (effect != nullptr)
        isChanged |= effect->onTickFrame();
}

void SafePiece::open()
{
    if (switchImagePiece->isOpen())
        return;

    switchImagePiece->open();

    isChanged = true;
}

void SafePiece::lock()
{
    if (switchImagePiece->isLock())
        return;

    switchImagePiece->lock();

    isChanged = true;
}

bool SafePiece::isOpen() const
{
    return switchImagePiece->isOpen();
}

bool SafePiece::isLock() const
{
    return switchImagePiece->isLock();
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

void SafePiece::fillRect(QPainter &painter)
{
    painter.setOpacity(1);

    isNearMine() ? painter.fillRect(rect, Qt::white)
                 : painter.fillRect(rect, Qt::black);
}

} // MineSweeper
