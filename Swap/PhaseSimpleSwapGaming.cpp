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
#include "PhaseSimpleSwapGaming.h"
#include "fifteen/FifteenPieceMover.h"

PhaseSimpleSwapGaming::PhaseSimpleSwapGaming(BoardPointer board, const QPoint &swapTargetPos, PhaseType nextPhase, int slideFrameCount, QObject *parent) :
    IPhase(parent),
    board(board),
    swapTargetPos(swapTargetPos),
    nextPhase(nextPhase),
    slideFrameCount(slideFrameCount),
    isGameCleared(false)
{
    Q_ASSERT(slideFrameCount >= 0);
}

void PhaseSimpleSwapGaming::click(const QPoint &clickedPiecePos)
{
    if (isGameCleared | (clickedPiecePos == swapTargetPos))
        return;

    board->swapPiece(swapTargetPos, clickedPiecePos);

    isGameCleared = board->isClearerd();
}

void PhaseSimpleSwapGaming::onTickFrame()
{
    board->onTickFrame();

    if (isGameCleared) {
        if (--slideFrameCount == 0)
            emit toNextPhase(nextPhase);
    }
}

void PhaseSimpleSwapGaming::draw(QPainter &painter)
{
    board->draw(painter);
}

bool PhaseSimpleSwapGaming::canSave() const
{
    return true;
}

bool PhaseSimpleSwapGaming::canLoad() const
{
    return true;
}

QString PhaseSimpleSwapGaming::information() const
{
    return "";
}
