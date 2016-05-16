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
#include "SimpleSwapSaveData.h"
#include "GameSimpleSwap.h"

#include <QDataStream>
#include <QFile>

namespace Fifteen {

SimpleSwapSaveData::SimpleSwapSaveData(const QString &savedataFilePath) :
    savedataFilePath(savedataFilePath)
{
    Q_ASSERT(!savedataFilePath.isEmpty());
}

QString SimpleSwapSaveData::gameName() const
{
    return gameTypeName;
}

QString SimpleSwapSaveData::imageFilePath() const
{
    return imgFilePath;
}

QStringList SimpleSwapSaveData::informations() const
{
    return {
        QString("W%1 x H%2 : %3 pieces").arg(xy.width()).arg(xy.height()).arg(xy.width() * xy.height()),
        "",
        QString("Swap target position [%1, %2]").arg(swapTargetPos.x()).arg(swapTargetPos.y()),
    };
}

IGame *SimpleSwapSaveData::loadGame() const
{
    auto game = new GameSimpleSwap();

    if (!game->load(savedataFilePath))
        return nullptr;

    return game;
}

bool SimpleSwapSaveData::loadInfo()
{
    // GameSimpleSwap::save() の書き込み順と同調すること

    QFile file(savedataFilePath);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QDataStream stream(&file);

    stream >> gameTypeName;

    if (gameTypeName != GameSimpleSwap::gameName())
        return false;

    stream >> imgFilePath;
    stream >> xy;
    stream >> swapTargetPos;

    if (stream.status() != QDataStream::Ok)
        return false;

    return true;
}

bool SimpleSwapSaveData::isValid() const
{
    return true;
}

QIcon SimpleSwapSaveData::gameTypeIcon() const
{
    return QIcon(":/ico/gameSwap");
}

} // Fifteen
