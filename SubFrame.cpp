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
    SubFrame(subFrameRect, false, false)
{
}

SubFrame::SubFrame(const QRect &subFrameRect, bool adjustMaxWidth, bool adjustMaxHeight)
    : m_subFrameRect(subFrameRect)
    , m_sizeMaximized(adjustMaxWidth, adjustMaxHeight)
{
    Q_ASSERT(subFrameRect.isValid());
}

void SubFrame::draw(QPainter &painter)
{
    if (m_maxRect != painter.clipBoundingRect().toRect()) {
        m_maxRect = painter.clipBoundingRect().toRect();
        correctSize();
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
    if (m_sizeMaximized.first & m_sizeMaximized.second)
        return;

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

QPoint SubFrame::posOnImage() const
{
    return m_subFrameRect.topLeft() - m_maxRect.topLeft();
}

void SubFrame::correctPosition()
{
    if (m_subFrameRect.width() == m_maxRect.width())
        m_subFrameRect.moveLeft(m_maxRect.left());

    if (m_subFrameRect.height() == m_maxRect.height())
        m_subFrameRect.moveTop(m_maxRect.top());

    if (m_subFrameRect.left() < m_maxRect.left())
        m_subFrameRect.moveLeft(m_maxRect.left());

    if (m_subFrameRect.right() > m_maxRect.right())
        m_subFrameRect.moveRight(m_maxRect.right());

    if (m_subFrameRect.top() < m_maxRect.top())
        m_subFrameRect.moveTop(m_maxRect.top());

    if (m_subFrameRect.bottom() > m_maxRect.bottom())
        m_subFrameRect.moveBottom(m_maxRect.bottom());
}

void SubFrame::correctSize()
{
    if (m_sizeMaximized.first & m_sizeMaximized.second) {
        m_subFrameRect = m_maxRect;

        return;
    }

    if (m_sizeMaximized.first ^ m_sizeMaximized.second) {
        m_subFrameRect.setSize(m_subFrameRect.size().scaled(m_maxRect.size(), Qt::KeepAspectRatio));
        correctPosition();

        return;
    }

    const QPoint offset = m_maxRect.topLeft() - m_subFrameRect.topLeft();

    if (m_maxRect.contains(m_subFrameRect.translated(offset))) {
        correctPosition();
    } else {
        qWarning() << "The SubFrame will protrude. SubFrame =" << m_subFrameRect << "Max =" << m_maxRect;

        m_subFrameRect.setSize(m_subFrameRect.size().scaled(m_maxRect.size(), Qt::KeepAspectRatio));
        m_subFrameRect.setTopLeft(m_maxRect.topLeft());
    }
}
