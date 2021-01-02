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

QMap<BlockPiece::Info, QPixmap> BlockPiece::m_pixmapMap;

BlockPiece::BlockPiece(const QSize &size, QColor foreground, QColor lightLine, QColor darkLine)
    : m_foregroundColor(foreground)
    , m_lightLineColor(lightLine)
    , m_darkLineColor(darkLine)
{
    if (size.isEmpty())
        return;

    Info info(size, foreground.rgba(), lightLine.rgba(), darkLine.rgba());

    m_pixmap = m_pixmapMap[info];

    if (!m_pixmap.isNull())
        return;

    m_pixmap = QPixmap(size);

    QPainter painter(&m_pixmap);
    drawPiece(painter, m_pixmap.size());

    m_pixmapMap[info] = m_pixmap;
}

void BlockPiece::draw(QPainter &painter, const QPointF &pos)
{
    draw(painter, QRectF(pos, m_pixmap.size()));
}

void BlockPiece::draw(QPainter &painter, const QRectF &rect)
{
    painter.drawPixmap(rect, m_pixmap, m_pixmap.rect());
}

void BlockPiece::drawPiece(QPainter &painter, const QSize &targetSize)
{
    QPoint pos(0, 0);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);
    painter.setOpacity(1.0);

    painter.fillRect(QRect(pos, targetSize - QSize(1, 1)), m_foregroundColor);

    painter.setPen(m_darkLineColor);
    painter.drawRect(QRect(pos, targetSize - QSize(1, 1)));

    painter.setPen(m_lightLineColor);
    painter.drawRect(QRect(pos + QPoint(1, 1), targetSize - QSize(3, 3)));
}

BlockPiece::Info::Info(const QSize &size, QRgb rgba1, QRgb rgba2, QRgb rgba3)
    : m_sizeInt(size.width())
    , m_color1(rgba1)
    , m_color2(rgba3)
{
    Q_ASSERT(!size.isEmpty());

    m_sizeInt <<= 32;
    m_sizeInt |= size.height();

    m_color1 <<= 32;
    m_color1 |= rgba2;
}

bool BlockPiece::Info::operator<(const BlockPiece::Info &other) const
{
    if (m_sizeInt != other.m_sizeInt)
        return m_sizeInt < other.m_sizeInt;

    if (m_color1 != other.m_color1)
        return m_color1 < other.m_color1;

    return m_color2 < other.m_color2;
}
