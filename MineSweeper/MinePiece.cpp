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
#include "MinePiece.h"

#include "BlockPiece.h"

namespace MineSweeper {

MinePiece::MinePiece(const QRect &rect) :
    blockPiece(new BlockPiece(rect.size())),
    isOpened(false),
    isLocked(false),
    rect(rect),
    isChanged(true)
{
}

void MinePiece::draw(QPainter &painter)
{
    if (!isChanged)
        return;

    painter.save();
    painter.setOpacity(1.0);

    blockPiece->draw(painter, rect);

    if (effect != nullptr)
        effect->draw(painter, rect);

    painter.restore();

    isChanged = false;
}

void MinePiece::setEffect(EffectPointer effect)
{
    this->effect = effect;
}

void MinePiece::onTickFrame()
{
    if (effect != nullptr)
        isChanged |= effect->onTickFrame();
}

void MinePiece::open()
{
    if (isOpened | isLocked)
        return;

    blockPiece.reset(new BlockPiece(rect.size(), QColor(224, 128, 128), QColor(192, 96, 96), QColor(96, 0, 0)));
    isOpened = true;
    isChanged = true;
}

void MinePiece::lock()
{
    if (isOpen())
        return;

    blockPiece.reset(new BlockPiece(rect.size(), QColor(160, 160, 255), QColor(128, 128, 224), QColor(32, 32, 128)));
    isLocked = true;
    isChanged = true;
}

bool MinePiece::isOpen() const
{
    return isOpened;
}

bool MinePiece::isLock() const
{
    return isLocked;
}

bool MinePiece::isMine() const
{
    return true;
}

bool MinePiece::isNearMine() const
{
    return true;
}

bool MinePiece::isWall() const
{
    return false;
}

int MinePiece::numberOfAroundMines() const
{
    return 0;
}

} // MineSweeper
