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
#ifndef GAMEID_H
#define GAMEID_H

#include <QDataStream>
#include <QString>

class GameID
{
public:
    GameID();
    ~GameID() = default;

    static GameID fromQString(const QString &string);

    void swap(GameID &other);

    const QString &toString() const;

    void save(QDataStream &stream) const;
    void load(QDataStream &stream);

private:
    GameID(const QString &string);

    QString gameID;
};

#endif // GAMEID_H