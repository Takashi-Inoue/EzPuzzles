#ifndef ANIMATIONWARPMOVE_H
#define ANIMATIONWARPMOVE_H

#include "AbstractAnimation.h"

namespace Animation {

class WarpMove : public AbstractAnimation
{
public:
    WarpMove(const QRectF &from, const QRectF &to, int waitFrameCount);
    ~WarpMove() = default;

    // AbstractAnimation
    QRectF rect() override;

protected:
    QRectF rectFrom;
    QRectF rectTo;
};

} // Animation

#endif // ANIMATIONWARPMOVE_H
