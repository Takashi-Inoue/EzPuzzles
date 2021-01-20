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
#include "AbstractGameBoard.h"
#include "FifteenFactory.h"
#include "FifteenPieceMover.h"

#include <QDebug>

namespace Fifteen {

AbstractGameBoard::AbstractGameBoard(BoardInfoPointer boardInformation)
    : m_boardInfo(boardInformation)
{
    Q_CHECK_PTR(boardInformation);
}

void AbstractGameBoard::initPieces(QPixmap sourcePixmap, const QList<QPoint> &defaultPositions)
{
    Q_ASSERT(!sourcePixmap.isNull());

    m_pieces = Factory().createPieces(m_boardInfo, sourcePixmap, defaultPositions);
}

void AbstractGameBoard::draw(QPainter &painter)
{
    for (auto &piece : m_pieces)
        piece->draw(painter);
}

void AbstractGameBoard::onTickFrame()
{
    for (auto &piece : m_pieces)
        piece->onTickFrame();
}

void AbstractGameBoard::skipPiecesAnimation()
{
    for (auto &piece : m_pieces)
        piece->skipAnimation();
}

bool AbstractGameBoard::isGameCleared() const
{
    for (const auto &piece : m_pieces) {
        if (!piece->pos().isCorrect())
            return false;
    }

    return true;
}

BoardInfoPointer AbstractGameBoard::boardInfo() const
{
    return m_boardInfo;
}

QList<QPoint> AbstractGameBoard::defaultPiecesPos() const
{
    QList<QPoint> positions;

    for (const FifteenPiecePointer &piece : m_pieces)
        positions << piece->pos().defaultPos();

    return positions;
}

FifteenPiecePointer &AbstractGameBoard::piece(const QPoint &piecePos)
{
    return m_pieces[piecePos.y() * m_boardInfo->xCount() + piecePos.x()];
}

const FifteenPiecePointer &AbstractGameBoard::piece(const QPoint &piecePos) const
{
    return m_pieces[piecePos.y() * m_boardInfo->xCount() + piecePos.x()];
}

} // namespace Fifteen
