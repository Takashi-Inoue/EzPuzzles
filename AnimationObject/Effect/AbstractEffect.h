#ifndef ABSTRACTEFFECT_H
#define ABSTRACTEFFECT_H

#include "AnimationObject/AbstractAnimationObject.h"
#include <QPainter>

namespace Effect {

class AbstractEffect : public AbstractAnimationObject
{
public:
    AbstractEffect(int totalFrameCount, bool isLoop) :
        AbstractAnimationObject(totalFrameCount, isLoop)
    {
    }

    ~AbstractEffect() = default;

    virtual void draw(QPainter &, const QRectF &) = 0;
};

} // Effect

#endif // ABSTRACTEFFECT_H
