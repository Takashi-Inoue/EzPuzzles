#include "AnimationLineMove.h"

namespace Animation {

LineMove::LineMove(const QRectF &from, const QRectF &to, int totalFrameCount, bool isLoop) :
    AbstractAnimation(totalFrameCount, isLoop),
    rectFrom(from),
    rectTo(to)
{
    Q_ASSERT((from.width() >= 0) & (from.height() >= 0));
    Q_ASSERT((to.width()   >= 0) & (to.height()   >= 0));
}

QRectF LineMove::rect()
{
    double rev = nowFrame() / totalFrames();

    QPointF tl((rectTo.left()  - rectFrom.left())  * rev + rectFrom.left(),  (rectTo.top()    - rectFrom.top())    * rev + rectFrom.top());
    QPointF br((rectTo.right() - rectFrom.right()) * rev + rectFrom.right(), (rectTo.bottom() - rectFrom.bottom()) * rev + rectFrom.bottom());

    return QRectF(tl, br);
}

} // Animation
