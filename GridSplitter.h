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
#ifndef GRIDSPLITTER_H
#define GRIDSPLITTER_H

#include "ISubWidget.h"
#include <QList>
#include <QPen>

class GridSplitter : public ISubWidget
{
public:
    GridSplitter(int xSplitterCount, int ySplitterCount);
    GridSplitter(const QPen &pen, int xSplitterCount, int ySplitterCount);
    ~GridSplitter() = default;

    static QList<double> verticalSplitterPos(int width, int splitterCount);
    static QList<double> horizontalSplitterPos(int height, int splitterCount);
    static double verticalSplitterPos(int width, int splitterCount, int number);
    static double horizontalSplitterPos(int height, int splitterCount, int number);

    void draw(QPainter &) override;
    void mousePress(const QPoint &) override;
    void mouseRelease(const QPoint &) override;
    void mouseMove(const QPoint &) override;
    QPoint pos() const override;

private:
    QPen pen;
    int xSplitterCount;
    int ySplitterCount;
};

#endif // GRIDSPLITTER_H
