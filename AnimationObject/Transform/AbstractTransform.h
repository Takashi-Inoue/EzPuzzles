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
#ifndef ABSTRACTTRANSFORM_H
#define ABSTRACTTRANSFORM_H

#include "AnimationObject/AbstractAnimationObject.h"

#include <QSharedPointer>
#include <QTransform>

namespace Transform {

class AbstractTransform : public AbstractAnimationObject
{
public:
    AbstractTransform(int totalFrameCount);

    virtual void start(const QSizeF &) = 0;
    virtual QTransform transform() const = 0;
    QRectF mapInRect(const QRectF &baseRect);
};

} // Transform

using TransformPointer = QSharedPointer<Transform::AbstractTransform>;

#endif // ABSTRACTTRANSFORM_H
