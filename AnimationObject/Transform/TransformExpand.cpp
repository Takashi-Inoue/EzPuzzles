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
#include "TransformExpand.h"
#include <QDebug>

namespace Transform {

Expand::Expand(ExpandType expandType, int totalFrameCount)
    : AbstractTransform(totalFrameCount)
    , m_expandType(expandType)
{
}

void Expand::start(const QSizeF &size)
{
    m_maxSize = size;

    AbstractAnimationObject::resetFrame();
}

QTransform Expand::transform() const
{
    Scale scale = calcScale();
    QPointF pos = calcPos(scale);

    QTransform transform;

    transform.translate(pos.x(), pos.y());
    transform.scale(scale.first, scale.second);

    return transform;
}

Expand::Scale Expand::calcScale() const
{
    const double frameRatio = nowFrame() / totalFrames();

    switch (m_expandType) {
    case LeftToRight :
    case RightToLeft :
    case HorizontalFromCenter :
        return Scale(frameRatio, 1);

    case TopToBottom :
    case BottomToTop :
    case VerticalFromCenter :
        return Scale(1, frameRatio);

    case HorizontalToCenter :
        return Scale(1 - frameRatio, 1);

    case VerticalToCenter :
        return Scale(1, 1 - frameRatio);
    }
}

QPointF Expand::calcPos(const Scale &scale) const
{
    if ((m_expandType == LeftToRight) | (m_expandType == TopToBottom))
        return QPointF(0, 0);

    QSizeF newSize(m_maxSize.width() * scale.first, m_maxSize.height() * scale.second);

    switch (m_expandType) {
    case RightToLeft:
        return QPointF(m_maxSize.width() - newSize.width(), 0);

    case BottomToTop:
        return QPointF(0, m_maxSize.height() - newSize.height());

    case HorizontalToCenter:
    case HorizontalFromCenter:
        return QPointF((m_maxSize.width() - newSize.width()) / 2, 0);

    case VerticalToCenter:
    case VerticalFromCenter:
        return QPointF(0, (m_maxSize.height() - newSize.height()) / 2);

    default:
        return QPointF(0, 0);
    }
}

} // Transform
