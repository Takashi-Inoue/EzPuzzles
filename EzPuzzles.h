﻿/*
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
#ifndef EZPUZZLES_H
#define EZPUZZLES_H

#include <QSize>
#include <QString>

class EzPuzzles
{
public:
    EzPuzzles() = default;
    ~EzPuzzles() = default;

    enum GameType {
        UnkownGame,
        SimpleSlide,
        SimpleSwap,
        MineSweeper,
    };

    static QString gameName(GameType gameType);

    static QSize screenshotSize();

    static QString imageHistoryPath();
    static QString saveDirPath();
    static void createSaveDirPath();
};

#endif // EZPUZZLES_H
