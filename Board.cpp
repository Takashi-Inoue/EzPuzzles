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

    auto list =  from.x() == to.x() ? mover.slideVertical(from, to)
                                    : mover.slideHorizontal(from, to);

    addAnimationPieces(list);

    return list;
}

QList<Fifteen::PuzzlePiecePointer> Board::swapPiece(const QPoint &from, const QPoint &to)
{
    auto &pieceFrom = getPiece(from);
    auto &pieceTo = getPiece(to);

    pieceFrom.swap(pieceTo);

    pieceFrom->setPos(from);
    pieceTo->setPos(to);

    auto list = {pieceFrom, pieceTo};

    addAnimationPieces(list);

    return list;
}

void Board::draw(QPainter &painter)
{
    for (auto &piece : animationPieces)
        piece->draw(painter);

    auto itr = std::remove_if(animationPieces.begin(), animationPieces.end(), [](Fifteen::PuzzlePiecePointer &piece) {
        bool f1 = (piece->animation() == nullptr || piece->animation()->isFinishedAnimation());
        bool f2 = (piece->effect() == nullptr || piece->effect()->isFinishedAnimation());

        return (f1 & f2);
    });

    animationPieces.erase(itr, animationPieces.end());
}

void Board::onTickFrame()
{
    for (auto &piece : animationPieces)
        piece->onTickFrame();
}

void Board::skipPiecesAnimation()
{
    for (auto &piece : animationPieces)
        piece->skipAnimation();
}

BoardInfoPointer Board::boardInfo() const
{
    return boardInformation;
}

void Board::addAnimationPieces(QList<Fifteen::PuzzlePiecePointer> pieces)
{
    animationPieces += pieces;

    qSort(animationPieces);

    animationPieces.erase(std::unique(animationPieces.begin(), animationPieces.end()), animationPieces.end());
}

Fifteen::PuzzlePiecePointer &Board::getPiece(const QPoint &pos)
{
    return pieces[pos.y() * boardInformation->xCount() + pos.x()];
}
