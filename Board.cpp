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
#include "Board.h"
#include "fifteen/FifteenPieceMover.h"

#include <QDebug>

Board::Board(BoardInfoPointer boardInformation, QList<Fifteen::PuzzlePiecePointer> &pieces) :
    boardInformation(boardInformation),
    pieces(pieces)
{
    Q_CHECK_PTR(boardInformation);
}

QList<Fifteen::PuzzlePiecePointer> Board::slidePiece(const QPoint &from, const QPoint &to)
{
    if (from == to) {
        qDebug() << "Board::slidePiece() | from == to" << from << to;
        return {};
    }

    if ((from.x() != to.x()) & (from.y() != to.y())) {
        qDebug() << "Board::slidePiece() | from and to are not on straight line" << from << to;
        return {};
    }

    Fifteen::PieceMover mover(pieces, boardInformation->xCount());

    return from.x() == to.x() ? mover.slideVertical(from, to)
                              : mover.slideHorizontal(from, to);
}

QList<Fifteen::PuzzlePiecePointer> Board::swapPiece(const QPoint &from, const QPoint &to)
{
    auto &pieceFrom = getPiece(from);
    auto &pieceTo = getPiece(to);

    pieceFrom.swap(pieceTo);

    pieceFrom->setPos(from);
    pieceTo->setPos(to);

    return {pieceFrom, pieceTo};
}

void Board::draw(QPainter &painter)
{
    for (auto &piece : pieces)
        piece->draw(painter);
}

void Board::onTickFrame()
{
    for (auto &piece : pieces)
        piece->onTickFrame();
}

void Board::skipPiecesAnimation()
{
    for (auto &piece : pieces)
        piece->skipAnimation();
}

bool Board::isClearerd() const
{
    for (const auto &piece : pieces) {
        if (!piece->pos().isCorrect())
            return false;
    }

    return true;
}

BoardInfoPointer Board::boardInfo() const
{
    return boardInformation;
}

Fifteen::PuzzlePiecePointer &Board::getPiece(const QPoint &pos)
{
    return pieces[pos.y() * boardInformation->xCount() + pos.x()];
}
