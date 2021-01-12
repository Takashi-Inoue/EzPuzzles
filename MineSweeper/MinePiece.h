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

#include "AbstractMinePiece.h"

class IPiece;

namespace MineSweeper {

class MinePiece : public AbstractMinePiece
{
public:
    MinePiece(const QRect &destRect);

    // IMinePiece
    void setOpenPieceOpacity(double /*opacity*/) override {}

    void open() override;
    void lock() override;

    bool isOpened() const override;
    bool isLocked() const override;
    bool isMine() const override;
    bool isNearMine() const override;
    bool isWall() const override;

    int countAroundMines() const override;

protected:
    void drawImpl(QPainter &painter) override;

private:
    QScopedPointer<IPiece> m_blockPiece;
    bool m_isOpened;
    bool m_isLocked;
};

} // MineSweeper

#endif // MAINPIECE_H
