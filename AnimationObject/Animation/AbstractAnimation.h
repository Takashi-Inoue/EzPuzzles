#ifndef ABSTRACTANIMATION_H
#define ABSTRACTANIMATION_H

#include "AnimationObject/AbstractAnimationObject.h"

#include <QRectF>
#include <memory>

namespace Animation {

class AbstractAnimation : public AbstractAnimationObject
{
public:
    AbstractAnimation(int totalFrameCount, bool isLoop) :
        AbstractAnimationObject(totalFrameCount, isLoop)
    {
    }

    ~AbstractAnimation() = default;

    virtual void start(const QRectF &from, const QRectF &to) = 0;
    virtual QRectF rect() = 0;
};

} // Animation

typedef std::shared_ptr<Animation::AbstractAnimation> AnimationPointer;

#endif // ABSTRACTANIMATION_H
