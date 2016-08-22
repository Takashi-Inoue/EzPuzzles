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
#include "PhaseSimpleSwapEnding.h"
#include "AnimationObject/Effect/EffectRoundMoveFrame.h"

PhaseSimpleSwapEnding::PhaseSimpleSwapEnding(QList<Fifteen::PuzzlePiecePointer> &pieces, PhaseType nextPhase) :
    pieces(pieces),
    nextPhase(nextPhase),
    frameMoveCount(120)
{
    for (auto &piece : pieces) {
        auto frame = std::make_shared<Effect::RoundMoveFrame>(2, Qt::transparent, Qt::transparent, QColor(32, 32, 32, 192), QColor(160, 160, 160, 192),
                                                              Qt::TopEdge, 0, Effect::RoundMoveFrame::RightHandTurn, frameMoveCount, false);

        piece->setEffect(frame);
    }
}

void PhaseSimpleSwapEnding::click(const QPoint &)
{
    emit toNextPhase(nextPhase);
}

void PhaseSimpleSwapEnding::onTickFrame()
{
    for (auto &piece : pieces)
        piece->onTickFrame();

    if (frameMoveCount-- <= 0)
        emit toNextPhase(nextPhase);
}

void PhaseSimpleSwapEnding::draw(QPainter &painter)
{
    for (auto &piece : pieces)
        piece->draw(painter);
}

bool PhaseSimpleSwapEnding::canSave() const
{
    return false;
}

bool PhaseSimpleSwapEnding::canLoad() const
{
    return false;
}

QString PhaseSimpleSwapEnding::information() const
{
    return "Clear!";
}
