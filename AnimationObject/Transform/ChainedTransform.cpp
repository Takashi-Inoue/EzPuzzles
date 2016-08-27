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
#include "ChainedTransform.h"

namespace Transform {

ChainedTransform::ChainedTransform() :
    AbstractTransform(0),
    index(INT_MAX)
{

}

void ChainedTransform::addTransform(TransformPointer transform)
{
    Q_CHECK_PTR(transform);

    transforms << transform;
}

bool ChainedTransform::onTickFrame()
{
    for (int lim = transforms.size(); index < lim; ++index) {
        if (transforms[index]->onTickFrame())
            return true;
    }

    return false;
}

void ChainedTransform::skipAnimation()
{
    for (auto &transform : transforms)
        transform->skipAnimation();
}

bool ChainedTransform::isLoopAnimation()
{
    for (auto &transform : transforms) {
        if (transform->isLoopAnimation())
            return true;
    }

    return false;
}

bool ChainedTransform::isFinishedAnimation()
{
    return index >= transforms.size();
}

void ChainedTransform::start(const QSizeF &start)
{
    if (transforms.isEmpty())
        return;

    index = 0;

    for (auto &transform : transforms)
        transform->start(start);
}

QTransform ChainedTransform::transform() const
{
    if (index >= transforms.size())
        return QTransform();

    return transforms[index]->transform();
}

} // Transform
