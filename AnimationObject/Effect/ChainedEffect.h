/*
 * Copyright YEAR Takashi Inoue
 *
 * This file is part of APPNAME.
 *
 * APPNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * APPNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with APPNAME.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EFFECT_CHAINEDEFFECT_H
#define EFFECT_CHAINEDEFFECT_H

#include "AbstractEffect.h"

namespace Effect {

class ChainedEffect : public Effect::AbstractEffect
{
public:
    explicit ChainedEffect(bool isLoop);

    void addEffect(EffectPointer effect);

    bool onTickFrame() override;
    void skipAnimation() override;
    bool isFinishedAnimation() override;

    void draw(QPainter &, const QRectF &) override;

protected:
    QList<EffectPointer> m_effects;
    qsizetype m_effectIndex = 0;
};

} // namespace Effect

#endif // EFFECT_CHAINEDEFFECT_H
