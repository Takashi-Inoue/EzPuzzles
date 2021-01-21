/*
 * Copyright 2021 Takashi Inoue
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

#ifndef SWAP_GAMEBOARD_H
#define SWAP_GAMEBOARD_H

#include "Fifteen/AbstractGameBoard.h"

namespace Swap {

class GameBoard : public Fifteen::AbstractGameBoard
{
public:
    using AbstractGameBoard::AbstractGameBoard;

    void shufflePieces() override;
    void movePiece(const QPoint &from, const QPoint &to) override;
    int frameCountToMove() const override;
    bool isFinishedMoving() const override;

    void initPieces(QPixmap sourcePixmap, const QList<QPoint> &defaultPositions = {}) override;

protected:
    const QPoint m_swapTargetPos;
    FifteenPiecePointer m_lastMovedPiece;
};

} // namespace Swap

#endif // SWAP_GAMEBOARD_H
