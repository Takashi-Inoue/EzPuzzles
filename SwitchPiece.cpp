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
#include "SwitchPiece.h"

SwitchPiece::SwitchPiece() :
    isPieceOpen(false),
    opacity(1)
{
}

void SwitchPiece::open()
{
    isPieceOpen = true;
}

void SwitchPiece::close()
{
    isPieceOpen = false;
}

bool SwitchPiece::isOpen() const
{
    return isPieceOpen;
}

void SwitchPiece::draw(QPainter &painter, const QPoint &pos)
{
    isOpen() ? drawOpenPiece(painter, pos)
             : drawClosedPiece(painter, pos);
}

void SwitchPiece::draw(QPainter &painter, const QPoint &pos, const QSize &targetSize)
{
    isOpen() ? drawOpenPiece(painter, pos, targetSize)
             : drawClosedPiece(painter, pos, targetSize);
}

void SwitchPiece::setSize(const QSize &pieceSize)
{
    if (pieceSize != closedPixmap.size())
        createClosedPixmap(pieceSize);
}

void SwitchPiece::setOpenPieceOpacity(double opacity)
{
    Q_ASSERT(opacity >= 0 && opacity <= 1);
    this->opacity = opacity;
}

void SwitchPiece::createClosedPixmap(const QSize &pieceSize)
{
    closedPixmap = QPixmap(pieceSize);

    QPainter painter(&closedPixmap);

    drawClosedPiece(painter, QPoint(0, 0), pieceSize);

//    painter.setRenderHints(QPainter::Antialiasing);
//    drawCrossLine(painter);
}

void SwitchPiece::drawOpenPiece(QPainter &/*painter*/, const QPoint &/*pos*/)
{
    // empty
}

void SwitchPiece::drawOpenPiece(QPainter &/*painter*/, const QPoint &/*pos*/, const QSize &/*targetSize*/)
{
    // empty
}

void SwitchPiece::drawClosedPiece(QPainter &painter, const QPoint &pos)
{
    painter.drawPixmap(pos, closedPixmap);
}

void SwitchPiece::drawClosedPiece(QPainter &painter, const QPoint &pos, const QSize &targetSize)
{
    painter.fillRect(QRect(pos, targetSize - QSize(1, 1)), foreground);

    painter.setPen(darkLine);
    painter.drawRect(QRect(pos, targetSize - QSize(1, 1)));

    painter.setPen(lightLine);
    painter.drawRect(QRect(pos + QPoint(1, 1), targetSize - QSize(3, 3)));
}

void SwitchPiece::drawCrossLine(QPainter &painter, const QPoint &pos)
{
    drawCrossLine(painter, pos, closedPixmap.size());
}

void SwitchPiece::drawCrossLine(QPainter &painter, const QPoint &pos, const QSize &targetSize)
{
    painter.setPen((lightLine, 1));
    painter.setOpacity(0.5);
    painter.drawLine(pos.x() + 1, pos.y() + 1, targetSize.width() - 1, targetSize.height() - 1);
    painter.drawLine(targetSize.width() - 1, pos.y() + 1, pos.x() + 1, targetSize.height() - 1);
}
