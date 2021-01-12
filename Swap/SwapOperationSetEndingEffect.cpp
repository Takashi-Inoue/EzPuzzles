/*
 * Copyright 2021 Takashi Inoue
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

#include "SwapOperationSetEndingEffect.h"

#include "AnimationObject/Effect/TimeLimitedEffect.h"
#include "AnimationObject/Transform/ChainedTransform.h"
#include "AnimationObject/Transform/TransformExpand.h"
#include "AnimationObject/Transform/WaitTransform.h"

#include <QRandomGenerator>

namespace Swap {

OperationSetEndingEffect::OperationSetEndingEffect(BoardInfoPointer boardInfo)
    : m_boardInfo(boardInfo)
{
    Q_CHECK_PTR(boardInfo);
}

void OperationSetEndingEffect::exec(QList<FifteenPiecePointer> &pieces)
{
    for (FifteenPiecePointer &piece : pieces) {
        piece->setAnimation(nullptr);

        const int waitFrames = QRandomGenerator::global()->bounded(m_maxWaitFrames);

        piece->setEffect(QSharedPointer<Effect::TimeLimitedEffect>::create(waitFrames + m_expandFrames * 6, piece->effect()));

        auto waitTransform = QSharedPointer<Transform::WaitTransform>::create(waitFrames);
        auto expand1 = QSharedPointer<Transform::Expand>::create(Transform::Expand::HorizontalToCenter,   m_expandFrames * 2);
        auto expand2 = QSharedPointer<Transform::Expand>::create(Transform::Expand::HorizontalFromCenter, m_expandFrames * 2);
        auto expand3 = QSharedPointer<Transform::Expand>::create(Transform::Expand::HorizontalToCenter,   m_expandFrames);
        auto expand4 = QSharedPointer<Transform::Expand>::create(Transform::Expand::HorizontalFromCenter, m_expandFrames);
        auto chainedTransform = QSharedPointer<Transform::ChainedTransform>::create();

        chainedTransform->addTransform(waitTransform);
        chainedTransform->addTransform(expand1);
        chainedTransform->addTransform(expand2);
        chainedTransform->addTransform(expand3);
        chainedTransform->addTransform(expand4);

        piece->setTransform(chainedTransform);
        chainedTransform->start(m_boardInfo->rectFromPiecePos(piece->pos().currentPos()).size());
    }
}

int OperationSetEndingEffect::totalFrameCount() const
{
    return m_maxWaitFrames + m_expandFrames * 6;
}

} // namespace Swap
