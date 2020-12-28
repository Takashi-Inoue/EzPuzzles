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
#include "BrokenSaveData.h"
#include "SourceImage.h"

QString BrokenSaveData::gameTypeName() const
{
    return QStringLiteral("Broken data");
}

EzPuzzles::GameType BrokenSaveData::gameType() const
{
    return EzPuzzles::UnkownGame;
}

QIcon BrokenSaveData::gameTypeIcon() const
{
    static QIcon icon(QStringLiteral(":/icons/x"));
    return icon;
}

QSharedPointer<IGame> BrokenSaveData::loadGame()
{
    return nullptr;
}

QStringList BrokenSaveData::informations() const
{
    return {
        QStringLiteral("This data is broken.")
    };
}

bool BrokenSaveData::readInfo()
{
    return false;
}

bool BrokenSaveData::read()
{
    return false;
}

bool BrokenSaveData::write() const
{
    return false;
}
