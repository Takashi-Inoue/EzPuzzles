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
#ifndef MAINPIECE_H
#define MAINPIECE_H

#include "IMinePiece.h"
#include <memory>

class BlockPiece;

namespace MineSweeper {

class MinePiece : public IMinePiece
{
public:
    MinePiece(const QRect &rect);
    ~MinePiece() = default;

    // IMinePiece
    void draw(QPainter &painter) override;
    void setOpenPieceOpacity(double /*opacity*/) override {}
    void setEffect(EffectPointer) override;
    void onTickFrame() override;

    void open() override;
    void lock() override;

    bool isOpen() const override;
    bool isLock() const override;
    bool isMine() const override;
    bool isNearMine() const override;
    bool isWall() const override;

    int numberOfAroundMines() const override;

private:
    std::unique_ptr<IPiece> blockPiece;
    bool isOpened;
    bool isLocked;
    QRect rect;

    EffectPointer effect;

    bool isChanged;
};

} // MineSweeper

#endif // MAINPIECE_H
