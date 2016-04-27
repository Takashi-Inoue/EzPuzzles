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
#include "BlockPiece.h"

#include <QPainter>
#include <QDebug>

QMap<BlockPiece::Key, QPixmap> BlockPiece::pixmapMap;

BlockPiece::BlockPiece(const QSize &size, QColor foreground, QColor lightLine, QColor darkLine) :
    foregroundColor(foreground),
    lightLineColor(lightLine),
    darkLineColor(darkLine)
{
    if (size.isEmpty())
        return;

    Key key(size, foreground.rgba(), lightLine.rgba(), darkLine.rgba());

    pixmap = pixmapMap[key];

    if (!pixmap.isNull())
        return;

    pixmap = QPixmap(size);

    QPainter painter(&pixmap);
    drawPiece(painter, QPoint(0, 0), pixmap.size());

    pixmapMap[key] = pixmap;
}

void BlockPiece::draw(QPainter &painter, const QPoint &pos)
{
    Q_ASSERT(!pixmap.isNull());

    draw(painter, pos, pixmap.size());
}

void BlockPiece::draw(QPainter &painter, const QPoint &pos, const QSize &targetSize)
{
    painter.setOpacity(1.0);

    pixmap.isNull() ? drawPiece(painter, pos, targetSize)
                    : painter.drawPixmap(QRect(pos, targetSize), pixmap);
}

void BlockPiece::drawPiece(QPainter &painter, const QPoint &pos, const QSize &targetSize)
{
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);
    painter.setOpacity(1.0);

    painter.fillRect(QRect(pos, targetSize - QSize(1, 1)), foregroundColor);

    painter.setPen(darkLineColor);
    painter.drawRect(QRect(pos, targetSize - QSize(1, 1)));

    painter.setPen(lightLineColor);
    painter.drawRect(QRect(pos + QPoint(1, 1), targetSize - QSize(3, 3)));
}

BlockPiece::Key::Key(const QSize &size, QRgb rgba1, QRgb rgba2, QRgb rgba3) :
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

bool BlockPiece::Key::operator<(const BlockPiece::Key &other) const
{
    if (sizeInt != other.sizeInt)
        return sizeInt < other.sizeInt;

    if (color1 != other.color1)
        return color1 < other.color1;

    return color2 < other.color2;
}
