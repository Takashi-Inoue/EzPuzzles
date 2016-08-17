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
#ifndef ISAVEDATA_H
#define ISAVEDATA_H

#include "EzPuzzles.h"

#include <QStringList>
#include <QIcon>

class IGame;

class ISaveData
{
public:
    ISaveData() = default;
    virtual ~ISaveData() = default;

    virtual QIcon gameTypeIcon() const = 0;
    virtual bool isValid() const = 0;

    virtual bool loadInfo() = 0;

    virtual EzPuzzles::GameType gameType() const = 0;
    virtual QString gameTypeName() const = 0;
    virtual QString imageFilePath() const = 0;
    virtual QStringList informations() const = 0;

    virtual IGame *loadGame() = 0;
};

#endif // IGAMEINFO_H
