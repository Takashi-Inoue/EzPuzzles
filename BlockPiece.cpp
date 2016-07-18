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
#include "BlockPiece.h"

#include <QPainter>
#include <QDebug>

QMap<BlockPiece::Info, QPixmap> BlockPiece::pixmapMap;

BlockPiece::BlockPiece(const QSize &size, QColor foreground, QColor lightLine, QColor darkLine) :
    foregroundColor(foreground),
    lightLineColor(lightLine),
    darkLineColor(darkLine)
{
    if (size.isEmpty())
        return;

    Info info(size, foreground.rgba(), lightLine.rgba(), darkLine.rgba());

    pixmap = pixmapMap[info];

    if (!pixmap.isNull())
        return;

    pixmap = QPixmap(size);

    QPainter painter(&pixmap);
    drawPiece(painter, QPoint(0, 0), pixmap.size());

    pixmapMap[info] = pixmap;
}

void BlockPiece::draw(QPainter &painter, const QPointF &pos)
{
    draw(painter, QRectF(pos, pixmap.size()));
}

void BlockPiece::draw(QPainter &painter, const QRectF &rect)
{
    painter.setOpacity(1.0);

    pixmap.isNull() ? drawPiece(painter, rect.topLeft(), rect.size())
                    : painter.drawPixmap(rect, pixmap, pixmap.rect());
}

void BlockPiece::drawPiece(QPainter &painter, const QPointF &pos, const QSizeF &targetSize)
{
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);
    painter.setOpacity(1.0);

    painter.fillRect(QRectF(pos, targetSize - QSize(1, 1)), foregroundColor);

    painter.setPen(darkLineColor);
    painter.drawRect(QRectF(pos, targetSize - QSize(1, 1)));

    painter.setPen(lightLineColor);
    painter.drawRect(QRectF(pos + QPoint(1, 1), targetSize - QSize(3, 3)));
}

BlockPiece::Info::Info(const QSize &size, QRgb rgba1, QRgb rgba2, QRgb rgba3) :
    sizeInt(size.width()),
    color1(rgba1),
    color2(rgba3)
{
    Q_ASSERT(!size.isEmpty());

    sizeInt <<= 32;
    sizeInt |= size.height();

    color1 <<= 32;
    color1 |= rgba2;
}

bool BlockPiece::Info::operator<(const BlockPiece::Info &other) const
{
    if (sizeInt != other.sizeInt)
        return sizeInt < other.sizeInt;

    if (color1 != other.color1)
        return color1 < other.color1;

    return color2 < other.color2;
}
