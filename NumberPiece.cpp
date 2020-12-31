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
#include "NumberPiece.h"
#include <QPainterPath>
#include <QDebug>

NumberPiece::NumberPiece(int number, QColor color, QSize pieceSize) :
    m_number(number),
    m_color(color),
    m_pixmap(pieceSize)
{
    if (!pieceSize.isEmpty())
        init();
}

void NumberPiece::draw(QPainter &painter, const QPointF &pos)
{
    draw(painter, QRectF(pos, m_pixmap.size()));
}

void NumberPiece::draw(QPainter &painter, const QRectF &rect)
{
    if (rect.isEmpty())
        return;

    if (rect.size() != m_pixmap.size()) {
        m_pixmap = QPixmap(rect.size().toSize());
        init();
    }

    painter.setOpacity(1.0);
    painter.drawPixmap(rect.topLeft(), m_pixmap);
}

int NumberPiece::number() const
{
    return m_number;
}

void NumberPiece::init()
{
    qDebug() << "INIT NumberPiece :" << m_number;

    m_pixmap.fill(Qt::transparent);

    QFont font;
    font.setPixelSize(int(m_pixmap.height() * 0.9));
    font.setWeight(QFont::Black);

    QPainterPath path;
    path.addText(calcPathPos(font), font, QString::number(m_number));

    QPainter painter(&m_pixmap);

    painter.setRenderHints(QPainter::Antialiasing);
    painter.strokePath(path, QPen(Qt::white, 3));
    painter.fillPath(path, m_color);
}

QPointF NumberPiece::calcPathPos(const QFont &font)
{
    QFontMetricsF fMetricsF(font);

    qreal charWidth = fMetricsF.boundingRect(QString::number(m_number)).width();

    return QPointF((m_pixmap.width()  - charWidth) / 2.0
                 , (m_pixmap.height() + fMetricsF.capHeight()) / 2.0);
}
