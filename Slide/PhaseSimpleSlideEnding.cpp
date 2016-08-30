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
#include "PhaseSimpleSlideEnding.h"
#include "AnimationObject/Effect/EffectRoundMoveFrame.h"

namespace Slide {

PhaseSimpleSlideEnding::PhaseSimpleSlideEnding(QList<Fifteen::PuzzlePiecePointer> &pieces, Fifteen::PuzzlePiecePointer finalPiece, int currentBlankIndex, PhaseType nextPhase) :
    pieces(pieces),
    nextPhase(nextPhase),
    frameMoveCount(120)
{
    pieces[currentBlankIndex] = finalPiece;

    for (auto &piece : pieces) {
        auto frame = std::make_shared<Effect::RoundMoveFrame>(2, Qt::transparent, Qt::transparent, QColor(32, 32, 32, 192), QColor(160, 160, 160, 192),
                                                              Qt::TopEdge, 0, Effect::RoundMoveFrame::RightHandTurn, frameMoveCount, false);

        piece->setEffect(frame);
    }
}

void PhaseSimpleSlideEnding::click(const QPoint &)
{
    emit toNextPhase(nextPhase);
}

void PhaseSimpleSlideEnding::onTickFrame()
{
    for (auto &piece : pieces)
        piece->onTickFrame();

    if (frameMoveCount-- <= 0)
        emit toNextPhase(nextPhase);
}

void PhaseSimpleSlideEnding::draw(QPainter &painter)
{
    for (auto &piece : pieces)
        piece->draw(painter);
}

bool PhaseSimpleSlideEnding::canSave() const
{
    return false;
}

bool PhaseSimpleSlideEnding::canLoad() const
{
    return false;
}

QString PhaseSimpleSlideEnding::information() const
{
    return "Clear!";
}

} // Slide
