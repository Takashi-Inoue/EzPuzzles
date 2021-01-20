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

#ifndef ANIMATIONFACTORY_H
#define ANIMATIONFACTORY_H

#include "AnimationObject/Animation/AbstractAnimation.h"
#include "AnimationObject/Effect/AbstractEffect.h"
#include "AnimationObject/Transform/AbstractTransform.h"

class AnimationFactory
{
public:
    AnimationFactory() = default;
    ~AnimationFactory() = default;

    static AnimationPointer lineMove(int moveFrameCount);
    static AnimationPointer warpMove(int waitFrameCount);

    static EffectPointer fadeFrame(int graduallyFrames);
    static EffectPointer simpleFrame(int width, QColor outer, QColor inner);
    static EffectPointer graduallyBlinkFrame(int width
                                           , QColor outerStart, QColor innerStart
                                           , QColor outerEnd,   QColor innerEnd
                                           , int totalFrameCount, bool isLoop = true);

    static EffectPointer graduallyImage(int graduallyFrames, const QColor &background
                                      , const QPixmap &pixmap, const QRectF &sourceRect);

    static EffectPointer waitEffect(int waitFrameCount);

    static TransformPointer lotateXTransform(int lotateFrameCount);
};

#endif // ANIMATIONFACTORY_H
