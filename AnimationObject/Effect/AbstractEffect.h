#ifndef ABSTRACTEFFECT_H
#define ABSTRACTEFFECT_H

#include "AnimationObject/AbstractAnimationObject.h"

#include <QPainter>
#include <memory>

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

typedef std::shared_ptr<Effect::AbstractEffect> EffectPointer;

#endif // ABSTRACTEFFECT_H
