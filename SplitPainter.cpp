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
#include "SplitPainter.h"

#include <QPainter>
#include <QDebug>

SplitPainter::SplitPainter(int xSplitterCount, int ySplitterCount) :
    pen(QColor(255, 0, 0, 128), 1),
    xSplitterCount(xSplitterCount),
    ySplitterCount(ySplitterCount)
{
    Q_ASSERT(xSplitterCount >= 0 && ySplitterCount >= 0);
}

SplitPainter::SplitPainter(const QPen &pen, int xSplitterCount, int ySplitterCount) :
    pen(pen),
    xSplitterCount(xSplitterCount),
    ySplitterCount(ySplitterCount)
{
    Q_ASSERT(xSplitterCount >= 0 && ySplitterCount >= 0);
}

QList<double> SplitPainter::verticalSplitterPos(int width, int splitterCount)
{
    QList<double> result;

    double partW = (width - splitterCount) / static_cast<double>(splitterCount + 1);

    for (int x = 1; x <= splitterCount; ++x)
        result << x * partW + x - 1;

    return result;
}

QList<double> SplitPainter::horizontalSplitterPos(int height, int splitterCount)
{
    QList<double> result;

    double partH = (height - splitterCount) / static_cast<double>(splitterCount + 1);

    for (int y = 1; y <= splitterCount; ++y)
        result << y * partH + y - 1;

    return result;
}

double SplitPainter::verticalSplitterPos(int width, int splitterCount, int number)
{
    double partV = (width - splitterCount) / static_cast<double>(splitterCount + 1);

    return number * partV + number - 1;
}

double SplitPainter::horizontalSplitterPos(int height, int splitterCount, int number)
{
    double partH = (height - splitterCount) / static_cast<double>(splitterCount + 1);

    return number * partH + number - 1;
}

//        painter.setPen(QPen(QColor(128, 128, 128, 128), 1));
//        painter.drawLine(QPointF(dx - 1, 0), QPointF(dx - 1, rect.bottom()));
//        painter.drawLine(QPointF(dx + 1, 0), QPointF(dx + 1, rect.bottom()));
//        painter.setPen(QPen(QColor(64, 64, 64, 160), 1));
//        painter.drawLine(QPointF(dx, 0), QPointF(dx, rect.bottom()));

void SplitPainter::draw(QPainter &painter)
{
    painter.setPen(pen);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);

    QRectF rect = painter.clipBoundingRect();

    if (rect.isEmpty())
        rect = painter.viewport();

    QList<double> splitterXs = verticalSplitterPos(rect.width(),  xSplitterCount);
    QList<double> splitterYs = horizontalSplitterPos(rect.height(), ySplitterCount);

    for (double x : splitterXs) {
        x += rect.left();
        painter.drawLine(QPointF(x, 0), QPointF(x, rect.bottom()));
    }

    for (double y : splitterYs) {
        y += rect.top();
        painter.drawLine(QPointF(0, y), QPointF(rect.right(), y));
    }
}

void SplitPainter::mousePress(const QPoint &)
{
    // empty
}

void SplitPainter::mouseRelease(const QPoint &)
{
    // empty
}

void SplitPainter::mouseMove(const QPoint &)
{
    // empty
}

QPoint SplitPainter::pos() const
{
    return QPoint(0, 0);
}
