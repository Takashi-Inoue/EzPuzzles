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

#include "SwapGameBoard.h"

#include "AnimationFactory.h"
#include "Fifteen/IPuzzlePiece.h"

#include <QRandomGenerator>
#include <QDebug>

namespace Swap {

void GameBoard::shufflePieces()
{
    Q_ASSERT(!m_pieces.isEmpty());

    int countX = m_boardInfo->xCount();

    for (int i = int(m_pieces.size()) - 1; i > 0; --i) {
        int r = QRandomGenerator::global()->bounded(i);

        FifteenPiecePointer &lhs = m_pieces[i];
        FifteenPiecePointer &rhs = m_pieces[r];

        int lx = i % countX;
        int ly = i / countX;
        int rx = r % countX;
        int ry = r / countX;

        lhs.swap(rhs);
        lhs->setPosWithoutAnimation(QPoint(lx, ly));
        rhs->setPosWithoutAnimation(QPoint(rx, ry));
    }
}

void GameBoard::movePiece(const QPoint &from, const QPoint &to)
{
    FifteenPiecePointer &pieceFrom = piece(from);
    FifteenPiecePointer &pieceTo = piece(to);

    pieceFrom.swap(pieceTo);

    pieceFrom->setPos(from);
    pieceTo->setPos(to);

    m_lastMovedPiece = pieceFrom;
}

int GameBoard::frameCountToMove() const
{
    return 12;
}

bool GameBoard::isFinishedMoving() const
{
    return m_lastMovedPiece->transform()->isFinishedAnimation();
}

void GameBoard::initPieces(QPixmap sourcePixmap, const QList<QPoint> &defaultPositions)
{
    AbstractGameBoard::initPieces(sourcePixmap, defaultPositions);

    for (FifteenPiecePointer &piece : m_pieces) {
        piece->setAnimation(AnimationFactory::warpMove(frameCountToMove() / 2));
        piece->setTransform(AnimationFactory::lotateXTransform(frameCountToMove()));
        piece->setEffect(AnimationFactory::simpleFrame(2, "#C0202020", "#C0A0A0A0"));
    }
}

} // namespace Swap
