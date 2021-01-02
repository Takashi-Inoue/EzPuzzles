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

GridLines::GridLines(const QRectF &rect, ushort vLineCount, ushort hLineCount)
    : m_rect(rect)
    , m_vCellCount(vLineCount + 1)
    , m_hCellCount(hLineCount + 1)
{
    Q_ASSERT(vLineCount >= 0);
    Q_ASSERT(hLineCount >= 0);

    if (rect.isValid())
        createLines();
}

GridLines::GridLines(ushort vLineCount, ushort hLineCount)
    : GridLines(QRectF(), vLineCount, hLineCount)
{
}

void GridLines::setRect(const QRectF &rect)
{
    Q_ASSERT(rect.isValid());

    if (m_rect == rect)
        return;

    m_rect = rect;

    createLines();
}

void GridLines::draw(QPainter &painter) const
{
    painter.save();

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);

    for (qsizetype i = 1, lim = m_vLines.size() - 1; i < lim; ++i) {
        double x = m_vLines[i];
        painter.drawLine(QPointF(x, 0), QPointF(x, m_rect.bottom()));
    }

    for (qsizetype i = 1, lim = m_hLines.size() - 1; i < lim; ++i) {
        double y = m_hLines[i];
        painter.drawLine(QPointF(0, y), QPointF(m_rect.right(), y));
    }

    painter.restore();
}

QRectF GridLines::cellRect(const QPoint &cellPos) const
{
    if (m_rect.isEmpty())
        return QRectF();

    if ((uint(cellPos.x()) > m_vCellCount) | (uint(cellPos.y()) > m_hCellCount))
        return QRectF();

    QPointF tl(m_vLines.at(cellPos.x()),     m_hLines.at(cellPos.y()));
    QPointF br(m_vLines.at(cellPos.x() + 1), m_hLines.at(cellPos.y() + 1));

    return QRectF(tl, br);
}

QPoint GridLines::cellPosFromPixelPos(const QPoint &pixelPos) const
{
    if (!m_rect.contains(QPointF(pixelPos)))
        return QPoint(-1, -1);

    qreal x = (pixelPos.x() - m_rect.left()) * m_vCellCount / (m_rect.width()  + 1);
    qreal y = (pixelPos.y() - m_rect.top())  * m_hCellCount / (m_rect.height() + 1);

    return QPoint(int(x), int(y));
}

void GridLines::createLines()
{
    Q_ASSERT(m_rect.isValid());

    m_vLines.clear();
    m_hLines.clear();

    if (m_rect.isEmpty())
        return;

    const QSizeF cellSize(m_rect.width()  / m_vCellCount
                        , m_rect.height() / m_hCellCount);

    for (uint x = 0; x < m_vCellCount; ++x)
        m_vLines << x * cellSize.width() + m_rect.left();

    for (uint y = 0; y < m_hCellCount; ++y)
        m_hLines << y * cellSize.height() + m_rect.top();

    m_vLines << m_rect.right();
    m_hLines << m_rect.bottom();
}
