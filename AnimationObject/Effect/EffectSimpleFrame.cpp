#include "EffectSimpleFrame.h"
#include <QLinearGradient>

namespace Effect {

SimpleFrame::SimpleFrame(const QColor &color) :
    GraduallyBlinkFrame(color, color, 0, false)
{
}

SimpleFrame::SimpleFrame(int width, const QColor &outer, const QColor &inner) :
    GraduallyBlinkFrame(width, outer, inner, outer, inner, 0, false)
{
}

} // Effect
