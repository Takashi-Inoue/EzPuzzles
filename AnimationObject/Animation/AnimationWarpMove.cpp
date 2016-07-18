#include "AnimationWarpMove.h"

namespace Animation {

WarpMove::WarpMove(int waitFrameCount) :
    AbstractAnimation(waitFrameCount, false)
{
}

void WarpMove::start(const QRectF &from, const QRectF &to)
{
    rectFrom = from;
    rectTo = to;

    AbstractAnimationObject::resetFrame();
}

QRectF WarpMove::rect()
{
    return isFinishedAnimation() ? rectTo : rectFrom;
}

} // Animation
