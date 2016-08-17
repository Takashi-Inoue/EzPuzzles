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
#ifndef ABSTRACTWALLPIECE_H
#define ABSTRACTWALLPIECE_H

#include "IMinePiece.h"

namespace MineSweeper {

class WallPiece : public IMinePiece
{
public:
    WallPiece() = default;
    ~WallPiece() = default;

    // IPiece
    void draw(QPainter &/*painter*/, const QPointF &/*pos*/) override {}
    void draw(QPainter &/*painter*/, const QRectF &/*targetSize*/) override {}

    // ISwitchPiece
    void open() override {}
    void close() override {}
    void lock() override {}
    bool isOpen() const override;
    bool isLock() const override;
    void setOpenPieceOpacity(double /*opacity*/) override {}

    // IMinePiece
    void draw(QPainter &) override {}
    bool isMine() const override;
    bool isNearMine() const override;
    bool isWall() const override;
    int numberOfAroundMines() const override;
};

} // MineSweeper

#endif // ABSTRACTWALLPIECE_H
