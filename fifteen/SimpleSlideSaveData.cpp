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
#include "SimpleSlideSaveData.h"
#include "GameSimpleSlide.h"

#include <QDataStream>
#include <QFile>

namespace Fifteen {

SimpleSlideSaveData::SimpleSlideSaveData(const QString &savedataFilePath) :
    savedataFilePath(savedataFilePath)
{
    Q_ASSERT(!savedataFilePath.isEmpty());
}

QString SimpleSlideSaveData::gameName() const
{
    return gameTypeName;
}

QString SimpleSlideSaveData::imageFilePath() const
{
    return imgFilePath;
}

QStringList SimpleSlideSaveData::informations() const
{
    return {
        QString("W%1 x H%2 : %3 pieces").arg(xy.width()).arg(xy.height()).arg(xy.width() * xy.height()),
        "",
        QString("Default blank piece position [%1, %2]").arg(defaultBlankPos.x()).arg(defaultBlankPos.y()),
        QString("Current blank piece position [%1, %2]").arg(blankPos.x()).arg(blankPos.y()),
    };
}

IGame *SimpleSlideSaveData::loadGame() const
{
    auto game = new GameSimpleSlide();

    if (!game->load(savedataFilePath))
        return nullptr;

    return game;
}

bool SimpleSlideSaveData::loadInfo()
{
    // GameSimpleSlide の書き込み順と同調すること

    QFile file(savedataFilePath);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QDataStream stream(&file);

    stream >> gameTypeName;

    if (gameTypeName != GameSimpleSlide::gameName())
        return false;

    stream >> imgFilePath;
    stream >> xy;
    stream >> defaultBlankPos;
    stream >> blankPos;

    if (stream.status() != QDataStream::Ok)
        return false;

    return true;
}

bool SimpleSlideSaveData::isValid() const
{
    return true;
}

QIcon SimpleSlideSaveData::gameTypeIcon() const
{
    return QIcon(":/ico/gameSimpleSlide");
}

} // Fifteen
