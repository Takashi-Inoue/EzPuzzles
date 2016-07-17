#ifndef ABSTRACTANIMATION_H
#define ABSTRACTANIMATION_H

#include "AnimationObject/AbstractAnimationObject.h"
#include <QRectF>

namespace Animation {

class AbstractAnimation : public AbstractAnimationObject
{
public:
    AbstractAnimation(int totalFrameCount, bool isLoop) :
        AbstractAnimationObject(totalFrameCount, isLoop)
    {
    }

    ~AbstractAnimation() = default;

    virtual QRectF rect() = 0;
};

} // Animation

#endif // ABSTRACTANIMATION_H
