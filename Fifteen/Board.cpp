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
#include "Board.h"
#include "FifteenPieceMover.h"

#include <QDebug>

namespace Fifteen {

Board::Board(BoardInfoPointer boardInformation, QList<FifteenPiecePointer> &pieces, std::shared_ptr<QReadWriteLock> rwlock) :
    boardInformation(boardInformation),
    m_pieces(pieces),
    rwlock(rwlock)
{
    Q_CHECK_PTR(boardInformation);
}

QList<FifteenPiecePointer> Board::slidePiece(const QPoint &from, const QPoint &to)
{
    if (from == to) {
        qDebug() << "Board::slidePiece() | from == to" << from << to;
        return {};
    }

    if ((from.x() != to.x()) & (from.y() != to.y())) {
        qDebug() << "Board::slidePiece() | from and to are not on straight line" << from << to;
        return {};
    }

    Fifteen::PieceMover mover(m_pieces, boardInformation->xCount());

    return from.x() == to.x() ? mover.slideVertical(from, to)
                              : mover.slideHorizontal(from, to);
}

QList<FifteenPiecePointer> Board::swapPiece(const QPoint &from, const QPoint &to)
{
    auto &pieceFrom = piece(from);
    auto &pieceTo = piece(to);

    pieceFrom.swap(pieceTo);

    pieceFrom->setPos(from);
    pieceTo->setPos(to);

    return {pieceFrom, pieceTo};
}

void Board::draw(QPainter &painter)
{
    for (auto &piece : m_pieces) {
        rwlock->lockForRead();
        piece->draw(painter);
        rwlock->unlock();
    }
}

void Board::execOperation(QSharedPointer<IOperationForPieces> operation)
{
    operation->exec(m_pieces);
}

void Board::onTickFrame()
{
    for (auto &piece : m_pieces)
        piece->onTickFrame();
}

void Board::skipPiecesAnimation()
{
    for (auto &piece : m_pieces)
        piece->skipAnimation();
}

bool Board::isCleared() const
{
    for (const auto &piece : m_pieces) {
        if (!piece->pos().isCorrect())
            return false;
    }

    return true;
}

BoardInfoPointer Board::boardInfo() const
{
    return boardInformation;
}

FifteenPiecePointer &Board::piece(const QPoint &piecePos)
{
    return m_pieces[piecePos.y() * boardInformation->xCount() + piecePos.x()];
}

} // namespace Fifteen
