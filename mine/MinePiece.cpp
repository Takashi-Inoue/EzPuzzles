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
#include "MinePiece.h"

namespace MineSweeper {

MinePiece::MinePiece()
{
}

bool MinePiece::isMine() const
{
    return true;
}

bool MinePiece::isNearMine() const
{
    return true;
}

bool MinePiece::isWall() const
{
    return false;
}

int MinePiece::numberOfAroundMines() const
{
    return 0;
}

void MinePiece::drawClosedPiece(QPainter &painter, const QPoint &pos, const QSize &targetSize)
{
    if (!isLocked) {
        SwitchPiece::drawClosedPiece(painter, pos, targetSize);
        return;
    }

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);

    painter.fillRect(QRect(pos, targetSize - QSize(1, 1)), lockedForeground);

    painter.setPen(lockedDarkLine);
    painter.drawRect(QRect(pos, targetSize - QSize(1, 1)));

    painter.setPen(lockedLightLine);
    painter.drawRect(QRect(pos + QPoint(1, 1), targetSize - QSize(3, 3)));
}

void MinePiece::drawOpenPiece(QPainter &painter, const QPoint &pos)
{
    drawOpenPiece(painter, pos, size);
}

void MinePiece::drawOpenPiece(QPainter &painter, const QPoint &pos, const QSize &targetSize)
{
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);

    painter.fillRect(QRect(pos, targetSize - QSize(1, 1)), explodeForeground);

    painter.setPen(explodeDarkLine);
    painter.drawRect(QRect(pos, targetSize - QSize(1, 1)));

    painter.setPen(explodeLightLine);
    painter.drawRect(QRect(pos + QPoint(1, 1), targetSize - QSize(3, 3)));
}

} // MineSweeper
