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

#include "ChainedEffect.h"

namespace Effect {

ChainedEffect::ChainedEffect(bool isLoop)
    : AbstractEffect(0, isLoop)
{
}

void ChainedEffect::addEffect(EffectPointer effect)
{
    Q_CHECK_PTR(effect);

    m_effects << effect;
}

bool ChainedEffect::onTickFrame()
{
    for (qsizetype lim = m_effects.size(); m_effectIndex < lim; ++m_effectIndex) {
        if (m_effects[m_effectIndex]->onTickFrame())
            return true;
    }

    return false;
}

void ChainedEffect::skipAnimation()
{
    if (isLoopAnimation())
        return;

    for (EffectPointer &effect : m_effects)
        effect->skipAnimation();
}

bool ChainedEffect::isFinishedAnimation()
{
    return isLoopAnimation() && m_effectIndex >= m_effects.size();
}

void ChainedEffect::draw(QPainter &painter, const QRectF &rect)
{
    if (m_effectIndex >= m_effects.size())
        return;

    m_effects[m_effectIndex]->draw(painter, rect);
}

} // namespace Effect
