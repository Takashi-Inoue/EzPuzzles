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
#include "GarbageCollector.h"

#include "IGame.h"
#include <QWriteLocker>
#include <QDebug>

GarbageCollector::~GarbageCollector()
{
    deleteGame();
}

void GarbageCollector::push(IGame *game)
{
    QWriteLocker wlock(&locker);

    if (!gameList.contains(game))
        gameList << game;
}

QString GarbageCollector::className() const
{
    return "GarbageCollector";
}

void GarbageCollector::execImpl()
{
    deleteGame();
}

void GarbageCollector::deleteGame()
{
    QWriteLocker wlock(&locker);

    for (IGame *game : gameList)
        delete game;

    gameList.clear();
}