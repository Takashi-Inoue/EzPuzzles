#ifndef SIMPLEFRAME_H
#define SIMPLEFRAME_H

#include "EffectGraduallyBlinkFrame.h"
#include <QColor>

namespace Effect {

class SimpleFrame : public GraduallyBlinkFrame
{
public:
    SimpleFrame(const QColor &color);
    SimpleFrame(int width, const QColor &outer, const QColor &inner);
    ~SimpleFrame() = default;
};

} // Effect

#endif // SIMPLEFRAME_H
