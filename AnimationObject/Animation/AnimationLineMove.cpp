#include "AnimationLineMove.h"

namespace Animation {

LineMove::LineMove(int totalFrameCount, bool isLoop) :
    AbstractAnimation(totalFrameCount, isLoop)
{
}

void LineMove::start(const QRectF &from, const QRectF &to)
{
    rectFrom = from;
    rectTo = to;

    AbstractAnimationObject::resetFrame();
}

QRectF LineMove::rect()
{
    double rev = nowFrame() / totalFrames();

    QPointF tl((rectTo.left()  - rectFrom.left())  * rev + rectFrom.left(),  (rectTo.top()    - rectFrom.top())    * rev + rectFrom.top());
    QPointF br((rectTo.right() - rectFrom.right()) * rev + rectFrom.right(), (rectTo.bottom() - rectFrom.bottom()) * rev + rectFrom.bottom());

    return QRectF(tl, br);
}

} // Animation
