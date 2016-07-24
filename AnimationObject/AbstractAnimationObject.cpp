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
#include "AbstractAnimationObject.h"

#include <QDebug>

AbstractAnimationObject::AbstractAnimationObject(int totalFrameCount, bool isLoop) :
    totalFrameCount(totalFrameCount),
    nowFrameCount(0),
    isLoop(isLoop)
{
    Q_ASSERT(totalFrameCount >= 0);
}

void AbstractAnimationObject::onTickFrame()
{
    nowFrameCount = isLoop ? ++nowFrameCount % totalFrameCount
                           : qMin(++nowFrameCount, totalFrameCount);
}

void AbstractAnimationObject::skipAnimation()
{
    if (!isLoop)
        nowFrameCount = totalFrameCount;
}

bool AbstractAnimationObject::isLoopAnimation()
{
    return isLoop;
}

bool AbstractAnimationObject::isFinishedAnimation()
{
    return !isLoop & (nowFrameCount >= totalFrameCount);
}

void AbstractAnimationObject::resetFrame()
{
    nowFrameCount = 0;
}

double AbstractAnimationObject::totalFrames() const
{
    return totalFrameCount;
}

double AbstractAnimationObject::nowFrame() const
{
    return nowFrameCount;
}
