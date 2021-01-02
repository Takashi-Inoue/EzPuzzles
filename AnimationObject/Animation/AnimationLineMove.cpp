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

void LineMove::start(const QRectF &from, const QRectF &to)
{
    m_rectFrom = from;
    m_rectTo = to;

    AbstractAnimationObject::resetFrame();
}

QRectF LineMove::rect()
{
    const qreal rev = nowFrame() / totalFrames();

    qreal left   = (m_rectTo.left()   - m_rectFrom.left())   * rev + m_rectFrom.left();
    qreal top    = (m_rectTo.top()    - m_rectFrom.top())    * rev + m_rectFrom.top();
    qreal right  = (m_rectTo.right()  - m_rectFrom.right())  * rev + m_rectFrom.right();
    qreal bottom = (m_rectTo.bottom() - m_rectFrom.bottom()) * rev + m_rectFrom.bottom();

    return QRectF(QPointF(left,  top), QPointF(right, bottom));
}

} // Animation
