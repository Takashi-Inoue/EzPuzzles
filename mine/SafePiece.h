﻿/*
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
#ifndef SAFEPIECE_H
#define SAFEPIECE_H

#include "IMinePiece.h"

#include <memory>

namespace MineSweeper {

class SafePiece : public IMinePiece
{
public:
    SafePiece(int numOfAroundMines, const QRect &destRect, const QPixmap &pixmap, const QRect &sourceRect);

    // ISwitchPiece
    void open() override;
    void close() override;
    void lock() override;
    bool isOpen() const override;
    bool isLock() const override;
    void setOpenPieceOpacity(double opacity) override;

    // IMinePiece
    void draw(QPainter &painter) override;
    bool isMine() const override;
    bool isNearMine() const override;
    bool isWall() const override;
    int numberOfAroundMines() const override;

protected:
    void fillRect(QPainter &painter);

    std::unique_ptr<ISwitchPiece> switchImagePiece;
    IPiece *numberPiece;

    int numOfAroundMines;
    QRect rect;

    bool isChanged;

    // -------------------- deleted functions --------------------
    // IPiece
    void draw(QPainter &, const QPointF &) override {}
    void draw(QPainter &, const QRectF &) override {}
};

} // MineSweeper

#endif // SAFEPIECE_H
