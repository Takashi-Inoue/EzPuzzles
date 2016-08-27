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

#include "AbstractTransform.h"

namespace Transform {

AbstractTransform::AbstractTransform(int totalFrameCount) :
    AbstractAnimationObject(totalFrameCount, false)
{
}

QRectF AbstractTransform::mapInRect(const QRectF &baseRect)
{
    QRectF rect = transform().mapRect(QRectF(QPoint(0, 0), baseRect.size()));

    return rect.marginsRemoved(QMarginsF(baseRect.left(), baseRect.top(), -baseRect.left(), -baseRect.top()));
}

} // Transform
