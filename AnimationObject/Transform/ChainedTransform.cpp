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
#include "ChainedTransform.h"

namespace Transform {

ChainedTransform::ChainedTransform()
    : AbstractTransform(0)
{
}

void ChainedTransform::addTransform(TransformPointer transform)
{
    Q_CHECK_PTR(transform);

    m_transforms << transform;
}

bool ChainedTransform::onTickFrame()
{
    for (qsizetype lim = m_transforms.size(); m_index < lim; ++m_index) {
        if (m_transforms[m_index]->onTickFrame())
            return true;
    }

    return false;
}

void ChainedTransform::skipAnimation()
{
    for (TransformPointer &transform : m_transforms)
        transform->skipAnimation();
}

bool ChainedTransform::isLoopAnimation()
{
    for (TransformPointer &transform : m_transforms) {
        if (transform->isLoopAnimation())
            return true;
    }

    return false;
}

bool ChainedTransform::isFinishedAnimation()
{
    return m_index >= m_transforms.size();
}

void ChainedTransform::start(const QSizeF &size)
{
    if (m_transforms.isEmpty())
        return;

    m_index = 0;

    for (TransformPointer &transform : m_transforms)
        transform->start(size);
}

QTransform ChainedTransform::transform() const
{
    if (m_index >= m_transforms.size())
        return QTransform();

    return m_transforms[m_index]->transform();
}

} // Transform
