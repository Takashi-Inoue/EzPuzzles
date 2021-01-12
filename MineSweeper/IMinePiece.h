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
#ifndef IMINEPIECE_H
#define IMINEPIECE_H

#include "ISwitchPiece.h"
#include "AnimationObject/Effect/AbstractEffect.h"

namespace MineSweeper {

class IMinePiece
{
public:
    IMinePiece() = default;
    virtual ~IMinePiece() = default;

    virtual void draw(QPainter &painter) = 0;
    virtual void setOpenPieceOpacity(double opacity) = 0;
    virtual void setEffect(EffectPointer) = 0;
    virtual void onTickFrame() = 0;

    virtual void open() = 0;
    virtual void lock() = 0;

    virtual bool isOpened() const = 0;
    virtual bool isLocked() const = 0;
    virtual bool isMine() const = 0;
    virtual bool isNearMine() const = 0;
    virtual bool isWall() const = 0;

    virtual int countAroundMines() const = 0;
};

using MinePiecePointer = QSharedPointer<IMinePiece>;

} // MineSweeper

#endif // IMINEPIECE_H
