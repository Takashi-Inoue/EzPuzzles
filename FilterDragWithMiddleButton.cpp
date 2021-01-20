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

#include "FilterDragWithMiddleButton.h"

#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>

#include <QDebug>

bool FilterDragWithMiddleButton::eventFilter(QObject *watched, QEvent *event)
{
    auto scrollArea = qobject_cast<QScrollArea *>(watched);

    if (scrollArea == nullptr)
        return QObject::eventFilter(watched, event);

    auto mouseEvent = static_cast<QMouseEvent *>(event);

    if (event->type() == QEvent::MouseMove)
        return onMouseMove(scrollArea, mouseEvent);

    if (event->type() == QEvent::MouseButtonPress)
        return onMousePress(scrollArea, mouseEvent);

    return false;
}

bool FilterDragWithMiddleButton::onMouseMove(QScrollArea * scrollArea, QMouseEvent *event)
{
    if (!(event->buttons() & Qt::MiddleButton))
        return false;

    qreal xScale = qreal(scrollArea->widget()->width())  / scrollArea->width();
    qreal yScale = qreal(scrollArea->widget()->height()) / scrollArea->height();
    QPoint subPoints = event->pos() - m_basePositions.first;

    subPoints.rx() = int(subPoints.x() * xScale);
    subPoints.ry() = int(subPoints.y() * yScale);

    QPoint newSliderValue = m_basePositions.second + subPoints;

    scrollArea->horizontalScrollBar()->setValue(newSliderValue.x());
    scrollArea->verticalScrollBar()->setValue(newSliderValue.y());

    return true;
}

bool FilterDragWithMiddleButton::onMousePress(QScrollArea *scrollArea, QMouseEvent *event)
{
    if (!(event->buttons() & Qt::MiddleButton))
        return false;

    QPoint sliderPos(scrollArea->horizontalScrollBar()->value()
                   , scrollArea->verticalScrollBar()->value());

    m_basePositions = qMakePair(event->pos(), sliderPos);

    return true;
}
