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
#include "AnimationLineMove.h"

namespace Animation {

LineMove::LineMove(int totalFrameCount, bool isLoop) :
    AbstractAnimation(totalFrameCount, isLoop)
{
}

void LineMove::start(const QRectF &from, const QRectF &to)
{
    rectFrom = from;
    rectTo = to;

    AbstractAnimationObject::resetFrame();
}

QRectF LineMove::rect()
{
    double rev = nowFrame() / totalFrames();

    QPointF tl((rectTo.left()  - rectFrom.left())  * rev + rectFrom.left(),  (rectTo.top()    - rectFrom.top())    * rev + rectFrom.top());
    QPointF br((rectTo.right() - rectFrom.right()) * rev + rectFrom.right(), (rectTo.bottom() - rectFrom.bottom()) * rev + rectFrom.bottom());

    return QRectF(tl, br);
}

} // Animation
