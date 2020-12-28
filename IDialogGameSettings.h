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
#ifndef IDIALOGGAMESETTINGS_H
#define IDIALOGGAMESETTINGS_H

#include <QSharedPointer>

class IGame;

class IDialogGameSettings
{
public:
    IDialogGameSettings() = default;
    virtual ~IDialogGameSettings() = default;

    virtual QSharedPointer<IGame> buildGame() const = 0;
};

#endif // IDIALOGGAMESETTINGS_H
