﻿/*
 * Copyright 2021 Takashi Inoue
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
#include "TimeLimitedEffect.h"

namespace Effect {

TimeLimitedEffect::TimeLimitedEffect(int limitFrames, EffectPointer limitedEffect) :
    AbstractEffect(limitFrames, false),
    m_limitedEffect(limitedEffect)
{
}

bool TimeLimitedEffect::onTickFrame()
{
    if (!AbstractAnimationObject::onTickFrame())
        return false;

    m_limitedEffect->onTickFrame();

    return true;
}

void TimeLimitedEffect::draw(QPainter &painter, const QRectF &rect)
{
    if (m_limitedEffect == nullptr)
        return;

    if (!isFinishedAnimation())
        m_limitedEffect->draw(painter, rect);
}

} // Effect
