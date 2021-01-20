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
#ifndef FIFTEEN_ABSTRACTGAMEBOARD_H
#define FIFTEEN_ABSTRACTGAMEBOARD_H

#include "BoardInformation.h"
#include "PuzzlePiece.h"

#include <QPainter>

namespace Fifteen {

class AbstractGameBoard
{
public:
    AbstractGameBoard(BoardInfoPointer boardInformation);
    virtual ~AbstractGameBoard() = default;

public:
    virtual void shufflePieces() = 0;
    virtual void movePiece(const QPoint &from, const QPoint &to) = 0;
    virtual int frameCountToMove() const = 0;
    virtual bool isFinishedMoving() const = 0;

    virtual void initPieces(QPixmap sourcePixmap, const QList<QPoint> &defaultPositions = {});

    void draw(QPainter &painter);
    void onTickFrame();
    void skipPiecesAnimation();

    bool isGameCleared() const;

    BoardInfoPointer boardInfo() const;
    QList<QPoint> defaultPiecesPos() const;
    FifteenPiecePointer &piece(const QPoint &piecePos);
    const FifteenPiecePointer &piece(const QPoint &piecePos) const;

protected:
    BoardInfoPointer m_boardInfo;
    QList<FifteenPiecePointer> m_pieces;
};

} // namespace Fifteen

using GameBoardPtr = QSharedPointer<Fifteen::AbstractGameBoard>;

#endif // FIFTEEN_ABSTRACTBOARD_H
