#include "CompositeEffect.h"

namespace Effect {

CompositeEffect::CompositeEffect() :
    AbstractEffect(0, false)
{
}

void CompositeEffect::addEffect(EffectPointer effect)
{
    if (!effects.contains(effect))
        effects << effect;
}

void CompositeEffect::onTickFrame()
{
    for (const auto &effect : effects)
        effect->onTickFrame();
}

void CompositeEffect::skipAnimation()
{
    for (const auto &effect : effects)
        effect->skipAnimation();
}

bool CompositeEffect::isLoopAnimation()
{
    for (const auto &effect : effects) {
        if (effect->isLoopAnimation())
            return true;
    }

    return false;
}

bool CompositeEffect::isFinishedAnimation()
{
    for (const auto &effect : effects) {
        if (!effect->isFinishedAnimation())
            return false;
    }

    return true;
}

void CompositeEffect::draw(QPainter &painter, const QRectF &rect)
{
    for (const auto &effect : effects)
        effect->draw(painter, rect);
}

} // Effect
