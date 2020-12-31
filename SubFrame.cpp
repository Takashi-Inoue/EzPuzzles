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

SubFrame::SubFrame(const QRect &subFrameRect) :
    m_subFrameRect(subFrameRect)
{
    Q_ASSERT(subFrameRect.isValid());
}

void SubFrame::draw(QPainter &painter)
{
    if (m_maxRect != painter.clipBoundingRect()) {
        m_maxRect = painter.clipBoundingRect().toRect();

        if (m_maxRect.width() < m_subFrameRect.width() || m_maxRect.height() < m_subFrameRect.height())
            qWarning() << "The SubFrame will protrude." << m_subFrameRect << m_maxRect;

        correctPosition();
    }

    painter.setPen(Qt::red);
    painter.drawRect(m_subFrameRect);
}

void SubFrame::mousePress(QMouseEvent *event)
{
    m_dragger.mouseDown(event->pos());
}

void SubFrame::mouseRelease(QMouseEvent *event)
{
    m_dragger.mouseRelease(event->pos());
}

void SubFrame::mouseMove(QMouseEvent *event)
{
    m_dragger.mouseMove(event->pos());

    if (!m_dragger.isDragging())
        return;

    m_subFrameRect.moveTopLeft(m_subFrameRect.topLeft() + m_dragger.sub());

    correctPosition();

    emit updated();
}

QPoint SubFrame::pos() const
{
    return m_subFrameRect.topLeft();
}

void SubFrame::correctPosition()
{
    if (m_subFrameRect.left() < m_maxRect.left())
        m_subFrameRect.moveLeft(m_maxRect.left());

    if (m_subFrameRect.right() > m_maxRect.right())
        m_subFrameRect.moveRight(m_maxRect.right());

    if (m_subFrameRect.top() < m_maxRect.top())
        m_subFrameRect.moveTop(m_maxRect.top());

    if (m_subFrameRect.bottom() > m_maxRect.bottom())
        m_subFrameRect.moveBottom(m_maxRect.bottom());
}
