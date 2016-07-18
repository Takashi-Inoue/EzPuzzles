#ifndef ANIMATIONLINEMOVE_H
#define ANIMATIONLINEMOVE_H

#include "AbstractAnimation.h"

namespace Animation {

class LineMove : public AbstractAnimation
{
public:
    LineMove(int totalFrameCount, bool isLoop);
    ~LineMove() = default;

    // AbstractAnimation
    void start(const QRectF &from, const QRectF &to) override;
    QRectF rect() override;

protected:
    QRectF rectFrom;
    QRectF rectTo;
};

} // Animation

#endif // ANIMATIONLINEMOVE_H
