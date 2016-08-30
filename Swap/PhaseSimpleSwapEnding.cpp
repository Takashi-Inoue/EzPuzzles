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
#include "PhaseSimpleSwapEnding.h"
#include "AnimationObject/Effect/TimeLimitedEffect.h"
#include "AnimationObject/Transform/ChainedTransform.h"
#include "AnimationObject/Transform/TransformExpand.h"
#include "AnimationObject/Transform/WaitTransform.h"

namespace Swap {

PhaseSimpleSwapEnding::PhaseSimpleSwapEnding(BoardInfoPointer boardInfo, QList<Fifteen::PuzzlePiecePointer> &pieces, PhaseType nextPhase) :
    boardInfo(boardInfo),
    pieces(pieces),
    nextPhase(nextPhase),
    nowFrame(0),
    mt(std::random_device()())
{
    for (auto &piece : pieces) {
        piece->setAnimation(nullptr);

        int waitFrames = mt() % maxWaitFrames;

        piece->setEffect(std::make_shared<Effect::TimeLimitedEffect>(waitFrames + quarterExpandFrames * 4, piece->effect()));

        auto waitTransform = std::make_shared<Transform::WaitTransform>(waitFrames);
        auto expand1 = std::make_shared<Transform::Expand>(Transform::Expand::HorizontalToCenter,   quarterExpandFrames * 2);
        auto expand2 = std::make_shared<Transform::Expand>(Transform::Expand::HorizontalFromCenter, quarterExpandFrames * 2);
        auto expand3 = std::make_shared<Transform::Expand>(Transform::Expand::HorizontalToCenter,   quarterExpandFrames);
        auto expand4 = std::make_shared<Transform::Expand>(Transform::Expand::HorizontalFromCenter, quarterExpandFrames);
        auto chainedTransform = std::make_shared<Transform::ChainedTransform>();

        chainedTransform->addTransform(waitTransform);
        chainedTransform->addTransform(expand1);
        chainedTransform->addTransform(expand2);
        chainedTransform->addTransform(expand3);
        chainedTransform->addTransform(expand4);

        piece->setTransform(chainedTransform);
        chainedTransform->start(boardInfo->rectFromPiecePos(piece->pos().currentPos()).size());
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

    if (nowFrame++ >= maxWaitFrames + quarterExpandFrames * 6)
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

} // Swap
