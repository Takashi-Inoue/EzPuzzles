﻿/*
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
#ifndef IPUZZLEPIECE_H
#define IPUZZLEPIECE_H

#include "Position.h"
#include "AnimationObject/Animation/AbstractAnimation.h"
#include "AnimationObject/Effect/AbstractEffect.h"
#include "AnimationObject/Transform/AbstractTransform.h"

#include <QSharedPointer>

namespace Fifteen {

class IPuzzlePiece
{
public:
    IPuzzlePiece() = default;
    virtual ~IPuzzlePiece() = default;

    virtual bool onTickFrame() = 0;
    virtual void skipAnimation() = 0;

    virtual void draw(QPainter &) = 0;
    virtual void setPos(const QPoint &pos) = 0;
    virtual void setPosWithoutAnimation(const QPoint &pos) = 0;
    virtual void setAnimation(AnimationPointer) = 0;
    virtual void setEffect(EffectPointer) = 0;
    virtual void setTransform(TransformPointer) = 0;
    virtual AnimationPointer animation() const = 0;
    virtual EffectPointer effect() const = 0;
    virtual TransformPointer transform() const = 0;

    virtual const Position &pos() const = 0;
};

} // Fifteen

using FifteenPiecePointer = QSharedPointer<Fifteen::IPuzzlePiece>;

#endif // IPUZZLEPIECE_H
