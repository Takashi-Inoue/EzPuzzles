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
#ifndef GAMECOREMINESWEEPER_H
#define GAMECOREMINESWEEPER_H

#include "GameCore.h"
#include "GameDataMineSweeper.h"

#include <QPair>
#include <random>

namespace MineSweeper {

class GameCoreMineSweeper : public GameCore
{
public:
    GameCoreMineSweeper(std::shared_ptr<GameDataMineSweeper> gameData);
    ~GameCoreMineSweeper() = default;

    void click(const QSize &fieldSize, const QPoint &cursorPos) override;
    void drawFinalImage(QPainter &dest) const override;

private:
    std::shared_ptr<GameDataMineSweeper> gameDataMineSweeper;
    QPixmap holeImg;

    mutable QList<QPair<QPoint, QMatrix>> matrixPairs;

    mutable std::mt19937 mt;
};

} // MineSweeper

#endif // GAMECOREMINESWEEPER_H
