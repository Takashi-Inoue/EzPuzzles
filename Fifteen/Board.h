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
#ifndef BOARD_H
#define BOARD_H

#include "BoardInformation.h"

#include "IOperationForPieces.h"
#include "PuzzlePiece.h"

#include <QPainter>
#include <QReadWriteLock>

namespace Fifteen {

class Board
{
public:
    Board(BoardInfoPointer boardInformation, QList<FifteenPiecePointer> &pieces, std::shared_ptr<QReadWriteLock> rwlock);
    ~Board() = default;

public:
    QList<FifteenPiecePointer> slidePiece(const QPoint &from, const QPoint &to);
    QList<FifteenPiecePointer> swapPiece(const QPoint &from, const QPoint &to);

    void draw(QPainter &painter);
    void execOperation(QSharedPointer<IOperationForPieces> operation);
    void onTickFrame();
    void skipPiecesAnimation();

    bool isCleared() const;

    BoardInfoPointer boardInfo() const;
    FifteenPiecePointer &piece(const QPoint &piecePos);

protected:
    void addAnimationPieces(QList<FifteenPiecePointer> pieces);

    BoardInfoPointer boardInformation;
    QList<FifteenPiecePointer> &m_pieces;

    std::shared_ptr<QReadWriteLock> rwlock;
};

} // namespace Fifteen

using BoardPointer = QSharedPointer<Fifteen::Board>;

#endif // BOARD_H
