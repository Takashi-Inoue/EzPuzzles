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
#include "GraduallyDrawer.h"
#include <QPainter>

GraduallyDrawer::GraduallyDrawer(const QPixmap &pixmap, Direction dir, int times) :
    pixmap(pixmap),
    direction(dir),
    times(times),
    counter(0)
{
    Q_ASSERT(!pixmap.isNull());
}

void GraduallyDrawer::draw(QPainter &dest)
{
    switch (direction) {
    case LeftToRight:
        drawLeftToRight(dest);
        break;

    case RightToLeft:
        drawRightToLeft(dest);
        break;

    case TopToBottom:
        drawTopToBottom(dest);
        break;

    case BottomToTop:
        drawBottomToTop(dest);
        break;
    }
}

void GraduallyDrawer::drawLeftToRight(QPainter &dest)
{
    if (counter >= times) {
        dest.drawPixmap(0, 0, pixmap);
        return;
    }

    double right = (pixmap.width() * ++counter) / static_cast<double>(times);
    QRectF destRectF(0, 0, right, pixmap.height());

    dest.drawPixmap(destRectF, pixmap, destRectF);
}

void GraduallyDrawer::drawRightToLeft(QPainter &dest)
{
    if (counter >= times) {
        dest.drawPixmap(0, 0, pixmap);
        return;
    }

    int width = pixmap.width();

    double x = width - (width * ++counter) / static_cast<double>(times);
    QRectF destRectF(x, 0, width - x, pixmap.height());

    dest.drawPixmap(destRectF, pixmap, destRectF);
}

void GraduallyDrawer::drawTopToBottom(QPainter &dest)
{
    if (counter >= times) {
        dest.drawPixmap(0, 0, pixmap);
        return;
    }

    double bottom = (pixmap.height() * ++counter) / static_cast<double>(times);
    QRectF destRectF(0, 0, pixmap.width(), bottom);

    dest.drawPixmap(destRectF, pixmap, destRectF);
}

void GraduallyDrawer::drawBottomToTop(QPainter &dest)
{
    if (counter >= times) {
        dest.drawPixmap(0, 0, pixmap);
        return;
    }

    int height = pixmap.height();

    double y = height - (height * ++counter) / static_cast<double>(times);
    QRectF destRectF(0, y, pixmap.width(), height - y);

    dest.drawPixmap(destRectF, pixmap, destRectF);
}
