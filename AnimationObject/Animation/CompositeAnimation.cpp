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
#include "CompositeAnimation.h"

namespace Animation {

CompositeAnimation::CompositeAnimation() :
    AbstractAnimation(0, false)
{
}

void CompositeAnimation::addAnimation(AnimationPointer animation)
{
    if (!animations.contains(animation))
        animations << animation;
}

bool CompositeAnimation::onTickFrame()
{
    bool isChanged = false;

    for (const auto &animation : animations)
        isChanged |= animation->onTickFrame();

    return isChanged;
}

void CompositeAnimation::skipAnimation()
{
    for (const auto &animation : animations)
        animation->skipAnimation();
}

bool CompositeAnimation::isLoopAnimation()
{
    for (const auto &effect : animations) {
        if (effect->isLoopAnimation())
            return true;
    }

    return false;
}

bool CompositeAnimation::isFinishedAnimation()
{
    for (const auto &animation : animations) {
        if (!animation->isFinishedAnimation())
            return false;
    }

    return true;
}

void CompositeAnimation::start(const QRectF &from, const QRectF &to)
{
    rectFrom = from;
    rectTo = to;

//    for (auto &animation : animations)
//        animation->resetFrame();
}

QRectF CompositeAnimation::rect()
{
    return QRectF();
}

} // Effect
