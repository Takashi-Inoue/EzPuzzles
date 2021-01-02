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
    m_totalFrameCount(totalFrameCount),
    m_nowFrameCount(0),
    m_isLoop(isLoop)
{
    Q_ASSERT(totalFrameCount >= 0);
}

bool AbstractAnimationObject::onTickFrame()
{
    if (m_isLoop) {
        m_nowFrameCount = ++m_nowFrameCount % m_totalFrameCount;

        return true;
    }

    if (m_nowFrameCount == m_totalFrameCount)
        return false;

    ++m_nowFrameCount;

    return true;
}

void AbstractAnimationObject::skipAnimation()
{
    if (!m_isLoop)
        m_nowFrameCount = m_totalFrameCount;
}

bool AbstractAnimationObject::isLoopAnimation()
{
    return m_isLoop;
}

bool AbstractAnimationObject::isFinishedAnimation()
{
    return !m_isLoop & (m_nowFrameCount >= m_totalFrameCount);
}

void AbstractAnimationObject::resetFrame()
{
    m_nowFrameCount = 0;
}

double AbstractAnimationObject::totalFrames() const
{
    return m_totalFrameCount;
}

double AbstractAnimationObject::nowFrame() const
{
    return m_nowFrameCount;
}
