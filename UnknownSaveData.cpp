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
#include "UnknownSaveData.h"
#include "SourceImage.h"

QString UnknownSaveData::gameTypeName() const
{
    return QStringLiteral("Not save data or broken.");
}

Application::GameType UnknownSaveData::gameType() const
{
    return Application::UnkownGame;
}

QIcon UnknownSaveData::gameTypeIcon() const
{
    return QIcon(QStringLiteral(":/icons/x"));
}

QSharedPointer<IGame> UnknownSaveData::loadGame()
{
    return nullptr;
}

QStringList UnknownSaveData::informations() const
{
    return {QStringLiteral("The game type cannot be read from this data.")};
}

bool UnknownSaveData::readInfo()
{
    return false;
}

bool UnknownSaveData::read()
{
    return false;
}

bool UnknownSaveData::write() const
{
    return false;
}
