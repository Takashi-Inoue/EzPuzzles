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
#include "GridLines.h"
#include <QDebug>

GridLines::GridLines(const QRectF &rect, int vLineCount, int hLineCount) :
    rect(rect),
    vLineCount(vLineCount),
    hLineCount(hLineCount)
{
    Q_ASSERT(vLineCount >= 0);
    Q_ASSERT(hLineCount >= 0);

    createLines();
}

GridLines::GridLines(int vLineCount, int hLineCount) :
    vLineCount(vLineCount),
    hLineCount(hLineCount)
{
    Q_ASSERT(vLineCount >= 0);
    Q_ASSERT(hLineCount >= 0);
}

void GridLines::setRect(const QRectF &rect)
{
    if (this->rect == rect)
        return;

    this->rect = rect;

    createLines();
}

void GridLines::draw(QPainter &painter) const
{
    painter.save();

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);

    for (int i = 1, lim = vLines.size() - 1; i < lim; ++i) {
        double x = vLines.at(i);
        double bottom = rect.bottom();

        painter.drawLine(QPointF(x, 0), QPointF(x, bottom));

    }

    for (int i = 1, lim = hLines.size() - 1; i < lim; ++i) {
        double y = hLines.at(i);
        double right = rect.right();

        painter.drawLine(QPointF(0, y), QPointF(right, y));
    }

    painter.restore();
}

QRectF GridLines::cellRect(const QPoint &cellPos) const
{
    if (rect.isEmpty())
        return QRectF();

    if ((cellPos.x() < 0) | (cellPos.y() < 0))
        return QRectF();

    if ((cellPos.x() > vLines.size()) | (cellPos.y() > hLines.size()))
        return QRectF();

    QPointF tl(vLines.at(cellPos.x()), hLines.at(cellPos.y()));
    QPointF br(vLines.at(cellPos.x() + 1),hLines.at(cellPos.y() + 1));

    return QRectF(tl, br);
}

QPoint GridLines::cellPosFromPixelPos(const QPoint &pixelPos) const
{
    // double to int の誤差によるバグが出るため QRectF.contains() は使用しない

    if ((pixelPos.x() < rect.left()) | (pixelPos.y() < rect.top()))
        return QPoint(-1, -1);

    QPoint pos((pixelPos.x() - rect.left()) * (vLineCount + 1) / rect.width(),
               (pixelPos.y() - rect.top())  * (hLineCount + 1) / rect.height());

    if ((pos.x() > vLineCount) | (pos.y() > hLineCount))
        return QPoint(-1, -1);

    return pos;
}

void GridLines::createLines()
{
    vLines.clear();
    hLines.clear();

    if (rect.isEmpty())
        return;

    vLines << rect.left();
    hLines << rect.top();

    double partW = (rect.width() - vLineCount) / (vLineCount + 1);

    for (int x = 1; x <= vLineCount; ++x)
        vLines << (x * partW) + (x - 1) + rect.left();

    double partH = (rect.height() - hLineCount) / (hLineCount + 1);

    for (int y = 1; y <= hLineCount; ++y)
        hLines << (y * partH) + (y - 1) + rect.top();

    vLines << rect.right();
    hLines << rect.bottom();
}
