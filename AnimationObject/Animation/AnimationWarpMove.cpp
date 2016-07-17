#include "AnimationWarpMove.h"

namespace Animation {

WarpMove::WarpMove(const QRectF &from, const QRectF &to, int waitFrameCount) :
    AbstractAnimation(waitFrameCount, false)
{
    Q_ASSERT((from.width() >= 0) & (from.height() >= 0));
    Q_ASSERT((to.width()   >= 0) & (to.height()   >= 0));
}

QRectF WarpMove::rect()
{
    return isFinishedAnimation() ? rectTo : rectFrom;
}

} // Animation
