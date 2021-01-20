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
#include "PhaseSimpleSlideGaming.h"
#include "Fifteen/FifteenPieceMover.h"

namespace Slide {

PhaseSimpleSlideGaming::PhaseSimpleSlideGaming(GameBoardPtr board, QPoint &currentBlankPos
                                             , const QPoint &defaultBlankPos, PhaseType nextPhase
                                             , QObject *parent) :
    AbstractPhase(nextPhase, parent),
    m_board(board),
    m_currentBlankPos(currentBlankPos),
    m_defaultBlankPos(defaultBlankPos),
    m_isGameCleared(false)
{
}

void PhaseSimpleSlideGaming::click(const QPoint &clickedPiecePos)
{
    if (m_isGameCleared | (clickedPiecePos == m_currentBlankPos))
        return;

    if ((clickedPiecePos.x() != m_currentBlankPos.x()) & (clickedPiecePos.y() != m_currentBlankPos.y()))
        return;

    m_board->movePiece(m_currentBlankPos, clickedPiecePos);

    m_currentBlankPos = clickedPiecePos;

    m_isGameCleared = m_board->isGameCleared();
}

void PhaseSimpleSlideGaming::onTickFrame()
{
    m_board->onTickFrame();

    if (m_isGameCleared && m_board->isFinishedMoving())
        emit toNextPhase(m_nextPhaseType);
}

void PhaseSimpleSlideGaming::draw(QPainter &painter)
{
    m_board->draw(painter);
}

bool PhaseSimpleSlideGaming::canSave() const
{
    return true;
}

bool PhaseSimpleSlideGaming::canLoad() const
{
    return true;
}

QString PhaseSimpleSlideGaming::information() const
{
    QSize xyCount = m_board->boardInfo()->xyCount();

    return QStringLiteral("%1 x %2, Blank Position Default[%3, %4] : Current[%5, %6]")
            .arg(xyCount.width()).arg(xyCount.height())
            .arg(m_defaultBlankPos.x() + 1).arg(m_defaultBlankPos.y() + 1)
            .arg(m_currentBlankPos.x() + 1).arg(m_currentBlankPos.y() + 1);
}

} // Slide
