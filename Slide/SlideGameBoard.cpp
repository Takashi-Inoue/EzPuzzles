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

#include "SlideGameBoard.h"

#include "AnimationFactory.h"
#include "Fifteen/FifteenPieceMover.h"
#include "Utility.h"

#include <QRandomGenerator>
#include <QDebug>

namespace Slide {

GameBoard::GameBoard(BoardInfoPointer boardInformation, QPoint &currentBlankPos)
    : AbstractGameBoard(boardInformation)
    , m_currentBlankPos(currentBlankPos)
{
}

void GameBoard::shufflePieces()
{
    auto randomPos = [](int &point, int highest, int currentPoint) {
        if ((point = QRandomGenerator::global()->bounded(highest)) == currentPoint)
            point = highest;
    };

    Qt::Orientation orientation = Qt::Horizontal;

    for (int i = 0, lim = m_boardInfo->pieceCount() * 4; i < lim; ++i) {
        orientation = (orientation == Qt::Horizontal) ? Qt::Vertical : Qt::Horizontal;
        QPoint nextBlankPos = m_currentBlankPos;

        orientation == Qt::Horizontal
                ? randomPos(nextBlankPos.rx(), m_boardInfo->xCount() - 1, m_currentBlankPos.x())
                : randomPos(nextBlankPos.ry(), m_boardInfo->yCount() - 1, m_currentBlankPos.y());

        auto slider = (orientation == Qt::Horizontal)
                      ? &Utility::slideHorizontal2Dlist<FifteenPiecePointer>
                      : &Utility::slideVertical2Dlist<FifteenPiecePointer>;

        QList<QPoint> changedPostions = slider(m_pieces, m_boardInfo->xCount()
                                             , m_currentBlankPos, nextBlankPos);

        for (const QPoint &pos : changedPostions)
            piece(pos)->setPosWithoutAnimation(pos);

        m_currentBlankPos = nextBlankPos;
    }
}

void GameBoard::movePiece(const QPoint &from, const QPoint &to)
{
    if (from == to)
        return;

    if ((from.x() != to.x()) & (from.y() != to.y()))
        return;

    Fifteen::PieceMover mover(m_pieces, m_boardInfo->xCount());

    from.x() == to.x() ? mover.slideVertical(from, to)
                       : mover.slideHorizontal(from, to);
}

int GameBoard::frameCountToMove() const
{
    return 20;
}

bool GameBoard::isFinishedMoving() const
{
    return piece(m_currentBlankPos)->animation()->isFinishedAnimation();
}

void GameBoard::initPieces(QPixmap sourcePixmap, const QList<QPoint> &defaultPositions)
{
    AbstractGameBoard::initPieces(sourcePixmap, defaultPositions);

    for (FifteenPiecePointer &piece : m_pieces) {
        piece->setAnimation(AnimationFactory::lineMove(frameCountToMove()));
        piece->setEffect(AnimationFactory::simpleFrame(2, "#C0202020", "#C0A0A0A0"));
    }
}

} // namespace Slide
