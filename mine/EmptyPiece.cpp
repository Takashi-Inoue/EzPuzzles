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
#include "EmptyPiece.h"
#include <QDebug>

namespace MineSweeper {

const QList<QColor> NumColor = {
    QColor(  0,   0, 255),
    QColor(  0, 160,   0),
    QColor(255,   0,   0),
    QColor(  0,   0, 128),
    QColor(128,   0,   0),
    QColor(  0, 128, 128),
    QColor( 64,  64,   0),
    QColor(  0,   0,   0),
};

EmptyPiece::EmptyPiece(QPixmap pixmap, int numberAroundMines) :
    pixmap(pixmap),
    numberAroundMines(numberAroundMines)
{
    Q_ASSERT(numberAroundMines >= 0 && numberAroundMines < 9);
}

bool EmptyPiece::isMine() const
{
    return false;
}

bool EmptyPiece::isNearMine() const
{
    return numberAroundMines > 0;
}

bool EmptyPiece::isWall() const
{
    return false;
}

int EmptyPiece::numberOfAroundMines() const
{
    return numberAroundMines;
}

void EmptyPiece::drawOpenPiece(QPainter &painter, const QPoint &pos)
{
    drawOpenPiece(painter, pos, pixmap.size());
}

void EmptyPiece::drawOpenPiece(QPainter &painter, const QPoint &pos, const QSize &targetSize)
{
    painter.setOpacity(1);

    if (!isNearMine()) {
        painter.fillRect(QRect(pos, targetSize), Qt::black);
        painter.setOpacity(SwitchPiece::opacity);
    } else {
        painter.fillRect(QRect(pos, targetSize), Qt::white);
        painter.setOpacity(0.5);
    }

    painter.drawPixmap(QRect(pos, targetSize), pixmap, pixmap.rect());
    painter.setOpacity(1);

    if (isNearMine()) {
        QString numberText = QString("%1").arg(numberAroundMines);

        QFont font = painter.font();
        font.setPixelSize(pixmap.height() * 0.8);

        QRect boundingRect;

        painter.setFont(font);
        painter.drawText(QRect(pos, targetSize), Qt::AlignCenter, numberText, &boundingRect);

        font.setStyleStrategy(QFont::ForceOutline);

        QPointF outlinePos(pos.x() + (targetSize.width()  - boundingRect.width())  / 2,
                           pos.y() + targetSize.height() * 0.8);

        QPainterPath path;
        path.addText(outlinePos, font, numberText);

        painter.setPen(QPen(Qt::white, 3));
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter.drawPath(path);

        font.setBold(true);

        painter.setFont(font);
        painter.setPen(QPen(NumColor.at(numberAroundMines - 1), 1));
        painter.drawText(QRect(pos, targetSize), Qt::AlignCenter, numberText);
    }
}

} // MineSweeper
