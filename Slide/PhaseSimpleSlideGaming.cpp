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

PhaseSimpleSlideGaming::PhaseSimpleSlideGaming(BoardPointer board, QPoint &currentBlankPos, const QPoint &defaultBlankPos, PhaseType nextPhase, int slideFrameCount, QObject *parent) :
    AbstractPhase(nextPhase, parent),
    board(board),
    currentBlankPos(currentBlankPos),
    defaultBlankPos(defaultBlankPos),
    slideFrameCount(slideFrameCount),
    isGameCleared(false)
{
    Q_ASSERT(slideFrameCount >= 0);
}

void PhaseSimpleSlideGaming::click(const QPoint &clickedPiecePos)
{
    if (isGameCleared | (clickedPiecePos == currentBlankPos))
        return;

    if ((clickedPiecePos.x() != currentBlankPos.x()) & (clickedPiecePos.y() != currentBlankPos.y()))
        return;

    board->slidePiece(currentBlankPos, clickedPiecePos);

    currentBlankPos = clickedPiecePos;

    isGameCleared = board->isCleared();
}

void PhaseSimpleSlideGaming::onTickFrame()
{
    board->onTickFrame();

    if (isGameCleared) {
        if (--slideFrameCount == 0)
            emit toNextPhase(m_nextPhaseType);
    }
}

void PhaseSimpleSlideGaming::draw(QPainter &painter)
{
    board->draw(painter);
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
    QSize xyCount = board->boardInfo()->xyCount();

    return QStringLiteral("%1 x %2, Blank Position Default[%3, %4] : Current[%5, %6]")
            .arg(xyCount.width()).arg(xyCount.height())
            .arg(defaultBlankPos.x() + 1).arg(defaultBlankPos.y() + 1)
            .arg(currentBlankPos.x() + 1).arg(currentBlankPos.y() + 1);
}

} // Slide
