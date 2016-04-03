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
#ifndef GRADUALLYDRAWER_H
#define GRADUALLYDRAWER_H

#include "IDrawer.h"
#include <QPixmap>

class GraduallyDrawer : public IDrawer
{
public:
    enum Direction {
        LeftToRight,
        RightToLeft,
        TopToBottom,
        BottomToTop,
    };

    GraduallyDrawer(const QPixmap &pixmap, Direction dir, int times);
    ~GraduallyDrawer() = default;

    void draw(QPainter &dest) override;

private:
    void drawLeftToRight(QPainter &dest);
    void drawRightToLeft(QPainter &dest);
    void drawTopToBottom(QPainter &dest);
    void drawBottomToTop(QPainter &dest);

    QPixmap pixmap;
    Direction direction;
    int times;
    int counter;
};

#endif // GRADUALLYDRAWER_H
