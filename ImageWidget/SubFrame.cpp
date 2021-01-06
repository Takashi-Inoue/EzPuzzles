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

SubFrame::SubFrame(const QSize &frameSize, const QSize &boundingSize, bool adjustContents
                 , const QPoint &framePos)
    : m_boundingSize(boundingSize)
    , m_adjustContents(adjustContents)
    , m_subFrameRect(framePos, frameSize)
{
    Q_ASSERT((frameSize.width()  <= m_boundingSize.width())
           & (frameSize.height() <= m_boundingSize.height()));

    correctPosition();
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

    QPoint offset = m_dragger.sub();

    if (m_adjustContents) {
        QSize scaledSize = m_boundingSize.scaled(m_destSize, Qt::KeepAspectRatio);

        offset = offset * m_boundingSize.width() / scaledSize.width();
    }

    m_subFrameRect.moveTopLeft(m_subFrameRect.topLeft() + offset);

    correctPosition();

    emit updated();
}

QPoint SubFrame::pos() const
{
    return m_subFrameRect.topLeft();
}

void SubFrame::drawImpl(QPainter &painter)
{
    painter.save();

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);
    painter.setPen(Qt::red);

    QRect destRect = painter.clipBoundingRect().toRect();

    m_destSize = destRect.size();

    QRectF drawRect = m_subFrameRect;

    if (m_adjustContents) {
        qreal lRatio = m_subFrameRect.left()   / qreal(m_boundingSize.width() - 1);
        qreal rRatio = m_subFrameRect.right()  / qreal(m_boundingSize.width() - 1);
        qreal tRatio = m_subFrameRect.top()    / qreal(m_boundingSize.height() - 1);
        qreal bRatio = m_subFrameRect.bottom() / qreal(m_boundingSize.height() - 1);

        drawRect.setLeft  (lRatio * (m_destSize.width()  - 1));
        drawRect.setRight (rRatio * (m_destSize.width()  - 1));
        drawRect.setTop   (tRatio * (m_destSize.height() - 1));
        drawRect.setBottom(bRatio * (m_destSize.height() - 1));
    }

    painter.drawRect(drawRect.translated(destRect.topLeft()));

    painter.restore();
}

void SubFrame::correctPosition()
{
    if (!m_adjustContents)
        return;

    if (m_subFrameRect.width() == m_boundingSize.width())
        m_subFrameRect.moveLeft(0);

    if (m_subFrameRect.height() == m_boundingSize.height())
        m_subFrameRect.moveTop(0);

    if (m_subFrameRect.left() < 0)
        m_subFrameRect.moveLeft(0);

    if (m_subFrameRect.right() >= m_boundingSize.width())
        m_subFrameRect.moveRight(m_boundingSize.width() - 1);

    if (m_subFrameRect.top() < 0)
        m_subFrameRect.moveTop(0);

    if (m_subFrameRect.bottom() >= m_boundingSize.height())
        m_subFrameRect.moveBottom(m_boundingSize.height() - 1);
}
