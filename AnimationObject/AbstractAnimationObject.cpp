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
