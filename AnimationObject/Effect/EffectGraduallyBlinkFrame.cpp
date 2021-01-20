/*
 * Copyright 2016 Takashi Inoue
 *
 * This file is part of EzPuzzles.
 *
 * EzPuzzles is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EzPuzzles is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EzPuzzles.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "EffectGraduallyBlinkFrame.h"

#include <QDebug>

namespace Effect {

QColor GraduallyBlinkFrame::currentColor(const QColor &start, const QColor &end) const
{
    if (totalFrames() == 0.0)
        return start;

    float pct = float((2 * nowFrame()) / totalFrames());

    if (pct > 1.0f)
        pct = 2 - pct;

    QColor color;

    color.setRedF  ((end.redF()   - start.redF())   * pct + start.redF());
    color.setGreenF((end.greenF() - start.greenF()) * pct + start.greenF());
    color.setBlueF ((end.blueF()  - start.blueF())  * pct + start.blueF());
    color.setAlphaF((end.alphaF() - start.alphaF()) * pct + start.alphaF());

    return color;
}

} // Effect
