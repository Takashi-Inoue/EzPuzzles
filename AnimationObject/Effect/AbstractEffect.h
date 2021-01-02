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
#ifndef ABSTRACTEFFECT_H
#define ABSTRACTEFFECT_H

#include "AnimationObject/AbstractAnimationObject.h"

#include <QPainter>
#include <QSharedPointer>

namespace Effect {

class AbstractEffect : public AbstractAnimationObject
{
public:
    using AbstractAnimationObject::AbstractAnimationObject;

    virtual void draw(QPainter &, const QRectF &) = 0;
};

} // Effect

using EffectPointer = QSharedPointer<Effect::AbstractEffect>;

#endif // ABSTRACTEFFECT_H
