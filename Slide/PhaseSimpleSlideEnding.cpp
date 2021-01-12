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
#include "AnimationObject/Effect/CompositeEffect.h"
#include "AnimationObject/Effect/EffectGraduallyImage.h"

namespace Slide {

PhaseSimpleSlideEnding::PhaseSimpleSlideEnding(BoardInfoPointer boardInfo, QList<FifteenPiecePointer> &pieces,
                                               QPixmap sourcePixmap, const QPoint &blankPos, PhaseType nextPhase, QObject *parent) :
    AbstractPhase(nextPhase, parent),
    pieces(pieces),
    nowFrame(0)
{
    for (auto &piece : pieces) {
        auto compositeEffect = QSharedPointer<Effect::CompositeEffect>::create();

        compositeEffect->addEffect(piece->effect());

        const QRectF &rect = boardInfo->rectFromPiecePos(piece->pos().defaultPos());
        compositeEffect->addEffect(QSharedPointer<Effect::GraduallyImage>::create(graduallyFrames, graduallyFrames, sourcePixmap, rect));

        piece->setEffect(compositeEffect);
    }

    FifteenPiecePointer &blankPiece = pieces[blankPos.y() * boardInfo->xCount() + blankPos.x()];
    const QRectF &rect = boardInfo->rectFromPiecePos(blankPiece->pos().defaultPos());

    blankPiece->setEffect(QSharedPointer<Effect::GraduallyImage>::create(0, graduallyFrames, sourcePixmap, rect));
}

void PhaseSimpleSlideEnding::click(const QPoint &)
{
    emit toNextPhase(m_nextPhaseType);
}

void PhaseSimpleSlideEnding::onTickFrame()
{
    for (auto &piece : pieces)
        piece->onTickFrame();

    if (++nowFrame > graduallyFrames * 2)
        emit toNextPhase(m_nextPhaseType);
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
