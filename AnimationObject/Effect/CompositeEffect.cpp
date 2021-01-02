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
#include "CompositeEffect.h"

namespace Effect {

CompositeEffect::CompositeEffect() :
    AbstractEffect(0, false)
{
}

void CompositeEffect::addEffect(EffectPointer effect)
{
    if (!m_effects.contains(effect))
        m_effects << effect;
}

bool CompositeEffect::onTickFrame()
{
    bool isChanged = false;

    for (const EffectPointer &effect : m_effects)
        isChanged |= effect->onTickFrame();

    return isChanged;
}

void CompositeEffect::skipAnimation()
{
    for (const EffectPointer &effect : m_effects)
        effect->skipAnimation();
}

bool CompositeEffect::isLoopAnimation()
{
    for (const EffectPointer &effect : m_effects) {
        if (effect->isLoopAnimation())
            return true;
    }

    return false;
}

bool CompositeEffect::isFinishedAnimation()
{
    for (const EffectPointer &effect : m_effects) {
        if (!effect->isFinishedAnimation())
            return false;
    }

    return true;
}

void CompositeEffect::draw(QPainter &painter, const QRectF &rect)
{
    for (const EffectPointer &effect : m_effects)
        effect->draw(painter, rect);
}

} // Effect
