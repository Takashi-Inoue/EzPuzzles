/*
 * Copyright YEAR Takashi Inoue
 *
 * This file is part of APPNAME.
 *
 * APPNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * APPNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with APPNAME.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef BOARD_H
#define BOARD_H

#include "BoardInformation.h"
#include "fifteen/PuzzlePiece.h"

#include <QPainter>
#include <QReadWriteLock>
#include <memory>

class Board
{
public:
    Board(BoardInfoPointer boardInformation, QList<Fifteen::PuzzlePiecePointer> &pieces, std::shared_ptr<QReadWriteLock> rwlock);
    ~Board() = default;

public:
    QList<Fifteen::PuzzlePiecePointer> slidePiece(const QPoint &from, const QPoint &to);
    QList<Fifteen::PuzzlePiecePointer> swapPiece(const QPoint &from, const QPoint &to);

    void draw(QPainter &painter);
    void onTickFrame();
    void skipPiecesAnimation();

    bool isClearerd() const;

    BoardInfoPointer boardInfo() const;

protected:
    void addAnimationPieces(QList<Fifteen::PuzzlePiecePointer> pieces);
    Fifteen::PuzzlePiecePointer &getPiece(const QPoint &pos);

    BoardInfoPointer boardInformation;
    QList<Fifteen::PuzzlePiecePointer> &pieces;

    std::shared_ptr<QReadWriteLock> rwlock;
};

typedef std::shared_ptr<Board> BoardPointer;

#endif // BOARD_H
