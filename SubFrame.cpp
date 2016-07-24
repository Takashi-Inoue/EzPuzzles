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
#include "SubFrame.h"

#include <QPainter>
#include <QDebug>

SubFrame::SubFrame(const QRect &rect) :
    rect(rect)
{
    Q_ASSERT(rect.isValid());
}

void SubFrame::draw(QPainter &painter)
{
    if (maxRect != painter.clipBoundingRect()) {
        maxRect = painter.clipBoundingRect().toRect();
        correctPosition();
    }

    painter.setPen(Qt::red);
    painter.drawRect(rect);
}

void SubFrame::mousePress(QMouseEvent *event)
{
    dragger.mouseDown(event->pos());
}

void SubFrame::mouseRelease(QMouseEvent *event)
{
    dragger.mouseRelease(event->pos());
}

void SubFrame::mouseMove(QMouseEvent *event)
{
    dragger.mouseMove(event->pos());

    if (!dragger.isDragging())
        return;

    rect.moveTopLeft(rect.topLeft() + dragger.sub());

    correctPosition();

    emit updated();
}

QPoint SubFrame::pos() const
{
    return rect.topLeft();
}

void SubFrame::correctPosition()
{
    if (rect.left() < maxRect.left())
        rect.moveLeft(maxRect.left());

    if (rect.right() > maxRect.right())
        rect.moveRight(maxRect.right());

    if (rect.top() < maxRect.top())
        rect.moveTop(maxRect.top());

    if (rect.bottom() > maxRect.bottom())
        rect.moveBottom(maxRect.bottom());
}
