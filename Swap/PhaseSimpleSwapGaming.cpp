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
#include "AnimationObject/Effect/CompositeEffect.h"
#include "AnimationObject/Effect/EffectGraduallyBlinkFrame.h"

PhaseSimpleSwapGaming::PhaseSimpleSwapGaming(BoardPointer board, QList<Fifteen::PuzzlePiecePointer> &pieces,
                                             const QPoint &swapTargetPos, PhaseType nextPhase, int slideFrameCount, QObject *parent) :
    IPhase(parent),
    board(board),
    pieces(pieces),
    swapTargetPos(swapTargetPos),
    nextPhase(nextPhase),
    slideFrameCount(slideFrameCount),
    isGameCleared(false)
{
    Q_ASSERT(slideFrameCount >= 0);


    auto graduallyFrame = std::make_shared<Effect::GraduallyBlinkFrame>(
                              8, QColor(255, 128, 64, 224), QColor(255, 255, 64, 0), QColor(255, 255, 64, 224), QColor(255, 128, 64, 0), 240, true);

    auto compositeEffect = std::make_shared<Effect::CompositeEffect>();

    auto piece = getPiece(swapTargetPos);

    compositeEffect->addEffect(piece->effect());
    compositeEffect->addEffect(graduallyFrame);

    piece->setEffect(compositeEffect);
}

void PhaseSimpleSwapGaming::click(const QPoint &clickedPiecePos)
{
    if (isGameCleared | (clickedPiecePos == swapTargetPos))
        return;

    board->swapPiece(swapTargetPos, clickedPiecePos);

    auto effect = getPiece(swapTargetPos)->effect();

    getPiece(swapTargetPos)->setEffect(getPiece(clickedPiecePos)->effect());
    getPiece(clickedPiecePos)->setEffect(effect);

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

Fifteen::PuzzlePiecePointer &PhaseSimpleSwapGaming::getPiece(const QPoint &pos)
{
    return pieces[pos.y() * board->boardInfo()->xCount() + pos.x()];
}
