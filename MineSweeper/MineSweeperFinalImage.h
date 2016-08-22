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
#ifndef MINESWEEPERFINALIMAGE_H
#define MINESWEEPERFINALIMAGE_H

#include "FinalImage.h"
#include "BoardInformation.h"
#include "MineField.h"

#include <random>

namespace MineSweeper {

class MineSweeperFinalImage : public FinalImage
{
public:
    MineSweeperFinalImage(QPixmap pixmap, MineFieldPointer mineField, BoardInfoPointer boardInfo);
    ~MineSweeperFinalImage() = default;

    void draw(QPainter &painter) override;

private:
    QList<QPointF> explodedCenters() const;

    MineFieldPointer mineField;
    BoardInfoPointer boardInfo;
    QPixmap holeImg;

    QList<QPair<QPoint, QMatrix>> matrixPairs;
    std::mt19937 mt;
};

} // MineSweeper

#endif // MINESWEEPERFINALIMAGE_H
