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
#include "NumberPiece.h"
#include <QDebug>

NumberPiece::NumberPiece(int number, QColor color, QSize pieceSize) :
    num(number),
    color(color),
    pixmap(pieceSize)
{
    if (!pieceSize.isEmpty())
        init();
}

void NumberPiece::draw(QPainter &painter, const QPoint &pos)
{
    draw(painter, pos, pixmap.size());
}

void NumberPiece::draw(QPainter &painter, const QPoint &pos, const QSize &targetSize)
{
    if (targetSize.isEmpty())
        return;

    if (targetSize != pixmap.size()) {
        pixmap = QPixmap(targetSize);
        init();
    }

    painter.setOpacity(1.0);
    painter.drawPixmap(pos, pixmap);
}

int NumberPiece::number() const
{
    return num;
}

void NumberPiece::init()
{
    qDebug() << "INIT NumberPiece :" << num;

    pixmap.fill(QColor(0, 0, 0, 0));

    QFont font;
    font.setPixelSize(pixmap.height() * 0.8);

    QPointF outlinePos = calcOutlinePos(font);
    font.setStyleStrategy(QFont::ForceOutline);

    QPainterPath path;
    path.addText(outlinePos, font, QString("%1").arg(num));

    font.setBold(true);

    QPainter painter(&pixmap);

    painter.setPen(QPen(Qt::white, 3));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.drawPath(path);

    painter.setFont(font);
    painter.setPen(color);
    painter.drawText(0, 0, pixmap.width(), pixmap.height(), Qt::AlignCenter, QString("%1").arg(num));
}

QPointF NumberPiece::calcOutlinePos(const QFont &font)
{
    QPainter painter(&pixmap);

    painter.setPen(QColor(0, 0, 0, 0));
    painter.setFont(font);

    QRect boundingRect;
    painter.drawText(0, 0, pixmap.width(), pixmap.height(), Qt::AlignCenter, QString("%1").arg(num), &boundingRect);

    return QPointF((pixmap.width() - boundingRect.width()) / 2, pixmap.height() * 0.8);
}
