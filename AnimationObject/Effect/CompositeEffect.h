#ifndef COMPOSITEEFFECT_H
#define COMPOSITEEFFECT_H

#include "AbstractEffect.h"

#include <QList>

namespace Effect {

class CompositeEffect : public AbstractEffect
{
public:
    CompositeEffect();
    ~CompositeEffect() = default;

    void addEffect(EffectPointer effect);

    // IAnimationObject
    void onTickFrame() override;
    void skipAnimation() override;
    bool isLoopAnimation() override;
    bool isFinishedAnimation() override;

    // AbstractEffect
    void draw(QPainter &painter, const QRectF &rect) override;

protected:
    QList<EffectPointer> effects;
};

} // Effect

#endif // COMPOSITEEFFECT_H
