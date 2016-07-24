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
#ifndef SELECTCELLGRID_H
#define SELECTCELLGRID_H

#include "ISubWidget.h"
#include "GridLines.h"
#include <QList>
#include <QPair>
#include <QPen>

class SelectCellGrid : public ISubWidget
{
public:
    SelectCellGrid(int vCellCount, int hCellCount);
    SelectCellGrid(const QPen &pen, int vSplitterCount, int hSplitterCount);
    ~SelectCellGrid() = default;

    void draw(QPainter &) override;
    void mousePress(QMouseEvent *) override;
    void mouseRelease(QMouseEvent *) override;
    void mouseMove(QMouseEvent *) override;
    void mouseEnter(QEvent *) override {}
    void mouseLeave(QEvent *) override;
    QPoint pos() const override;

    void setRandomSelect(bool isRandom);
    void setCellCount(int vCellCount, int hCellCount);
    const QPoint &selectedCellPos() const;

private:
    QPoint invalidPoint() const;

    QPen pen;
    GridLines gridLines;

    bool isPressed;
    QPoint onMousePos;
    QPoint selectedPos;

    bool isRandomSelect;
};

#endif // GRIDSPLITTER_H
