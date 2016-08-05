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
#include "PhaseSimpleSlideGaming.h"
#include "fifteen/FifteenPieceMover.h"

PhaseSimpleSlideGaming::PhaseSimpleSlideGaming(BoardPointer board, QPoint &currentBlankPos, PhaseType nextPhase, int slideFrameCount, QObject *parent) :
    IPhase(parent),
    board(board),
    blankPos(currentBlankPos),
    nextPhase(nextPhase),
    slideFrameCount(slideFrameCount),
    isGameCleared(false)
{
    Q_ASSERT(slideFrameCount >= 0);
}

void PhaseSimpleSlideGaming::click(const QPoint &clickedPiecePos)
{
    if (isGameCleared | (clickedPiecePos == blankPos))
        return;

    if ((clickedPiecePos.x() != blankPos.x()) & (clickedPiecePos.y() != blankPos.y()))
        return;

    board->slidePiece(blankPos, clickedPiecePos);

    blankPos = clickedPiecePos;

    isGameCleared = board->isClearerd();
}

void PhaseSimpleSlideGaming::onTickFrame()
{
    board->onTickFrame();

    if (isGameCleared) {
        if (--slideFrameCount == 0)
            emit toNextPhase(nextPhase);
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

bool PhaseSimpleSlideGaming::save(QDataStream &) const
{
    return true;
}

bool PhaseSimpleSlideGaming::load(QDataStream &)
{
    return true;
}

QString PhaseSimpleSlideGaming::information() const
{
    return "";
}
