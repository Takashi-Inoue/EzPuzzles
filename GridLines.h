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
#ifndef GRIDLINES_H
#define GRIDLINES_H

#include <QList>
#include <QPainter>

class GridLines
{
public:
    GridLines(const QRectF &rect, int vLineCount, int hLineCount);
    GridLines(int vLineCount, int hLineCount);
    virtual ~GridLines() = default;

    void setRect(const QRectF &rect);
    void draw(QPainter &painter) const;

    QRectF cellRect(const QPoint &cellPos) const;
    QPoint cellPosFromPixelPos(const QPoint &pixelPos) const;

protected:
    void createLines();

    QRectF rect;
    int vLineCount;
    int hLineCount;

    QList<double> vLines;
    QList<double> hLines;
};

#endif // GRIDLINES_H
