#ifndef ANIMATIONWARPMOVE_H
#define ANIMATIONWARPMOVE_H

#include "AbstractAnimation.h"

namespace Animation {

class WarpMove : public AbstractAnimation
{
public:
    WarpMove(int waitFrameCount);
    ~WarpMove() = default;

    // AbstractAnimation
    void start(const QRectF &from, const QRectF &to) override;
    QRectF rect() override;

protected:
    QRectF rectFrom;
    QRectF rectTo;
};

} // Animation

#endif // ANIMATIONWARPMOVE_H
