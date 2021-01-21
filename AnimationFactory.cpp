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

#include "AnimationFactory.h"

#include "AnimationObject/Animation/AnimationLineMove.h"
#include "AnimationObject/Animation/AnimationWarpMove.h"
#include "AnimationObject/Effect/EffectGradualFrame.h"
#include "AnimationObject/Effect/EffectGraduallyBlinkFrame.h"
#include "AnimationObject/Effect/EffectGradualImage.h"
#include "AnimationObject/Effect/EffectSimpleFrame.h"
#include "AnimationObject/Effect/WaitEffect.h"
#include "AnimationObject/Transform/ChainedTransform.h"
#include "AnimationObject/Transform/TransformExpand.h"

#include <QDebug>

AnimationPointer AnimationFactory::lineMove(int moveFrameCount)
{
    return QSharedPointer<Animation::LineMove>::create(moveFrameCount, false);
}

AnimationPointer AnimationFactory::warpMove(int waitFrameCount)
{
    return QSharedPointer<Animation::WarpMove>::create(waitFrameCount);
}

EffectPointer AnimationFactory::fadeFrame(int graduallyFrames)
{
    return QSharedPointer<Effect::GradualFrame>::create(
                2, QColor(32, 32, 32, 192), QColor(160, 160, 160, 192)
                 , Qt::transparent, Qt::transparent, graduallyFrames, false);
}

EffectPointer AnimationFactory::simpleFrame(int width, QColor outer, QColor inner)
{
    return QSharedPointer<Effect::SimpleFrame>::create(width, outer, inner);
}

EffectPointer AnimationFactory::graduallyBlinkFrame(int width
                                                  , QColor outerStart, QColor innerStart
                                                  , QColor outerEnd,   QColor innerEnd
                                                  , int totalFrameCount, bool isLoop)
{
    return QSharedPointer<Effect::GraduallyBlinkFrame>::create(
                width, outerStart, innerStart, outerEnd, innerEnd, totalFrameCount, isLoop);
}

EffectPointer AnimationFactory::graduallyImage(int graduallyFrames, const QColor &background
                                             , const QPixmap &pixmap, const QRectF &sourceRect)
{
    return QSharedPointer<Effect::GradualImage>::create(graduallyFrames, background
                                                      , pixmap, sourceRect);
}

EffectPointer AnimationFactory::waitEffect(int waitFrameCount)
{
    return QSharedPointer<Effect::WaitEffect>::create(waitFrameCount);
}

TransformPointer AnimationFactory::lotateXTransform(int lotateFrameCount)
{
    if (lotateFrameCount % 2 != 0)
        qWarning() << QStringLiteral("lotateFrameCount is not even number.");

    const int expandFrameCount = lotateFrameCount / 2;

    using Expand = Transform::Expand;
    using ExpandPtr = QSharedPointer<Expand>;

    auto chainTransform = new Transform::ChainedTransform();

    chainTransform->addTransform(ExpandPtr::create(Expand::HorizontalToCenter, expandFrameCount));
    chainTransform->addTransform(ExpandPtr::create(Expand::HorizontalFromCenter, expandFrameCount));

    return TransformPointer(chainTransform);
}
